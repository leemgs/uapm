
#define _GNU_SOURCE
#include "pm.h"
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>

static time_t last_input_mtime() {
    DIR* d = opendir("/dev/input");
    if (!d) return 0;
    struct dirent* ent;
    time_t newest = 0;
    while ((ent = readdir(d))) {
        if (strncmp(ent->d_name, "event", 5) != 0) continue;
        char path[256];
        snprintf(path, sizeof(path), "/dev/input/%s", ent->d_name);
        struct stat st;
        if (stat(path, &st) == 0) {
            if (st.st_mtime > newest) newest = st.st_mtime;
        }
    }
    closedir(d);
    return newest;
}

// Best effort: read one non-blocking event to refresh access time
static void poke_input_devices() {
    DIR* d = opendir("/dev/input");
    if (!d) return;
    struct dirent* ent;
    while ((ent = readdir(d))) {
        if (strncmp(ent->d_name, "event", 5) != 0) continue;
        char path[256];
        snprintf(path, sizeof(path), "/dev/input/%s", ent->d_name);
        int fd = open(path, O_RDONLY | O_NONBLOCK);
        if (fd >= 0) {
            struct input_event ev;
            read(fd, &ev, sizeof(ev)); // ignore errors
            close(fd);
        }
    }
    closedir(d);
}

bool user_inactive(unsigned threshold_sec, bool verbose) {
    time_t now = time(NULL);
    time_t mt = last_input_mtime();
    if (mt == 0) {
        if (verbose) fprintf(stderr, "[pm] no /dev/input access; using timer fallback\n");
        static time_t first = 0;
        if (first == 0) first = now;
        return (now - first) >= (time_t)threshold_sec;
    }
    // attempt to refresh
    poke_input_devices();
    // recompute
    mt = last_input_mtime();
    if (verbose) fprintf(stderr, "[pm] last input mtime=%ld, now=%ld, idle=%lds\n", (long)mt, (long)now, (long)(now-mt));
    return (now - mt) >= (time_t)threshold_sec;
}

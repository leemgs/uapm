
#define _GNU_SOURCE
#include "pm.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

bool request_suspend(bool verbose, bool dry_run) {
    if (dry_run) {
        if (verbose) fprintf(stderr, "[pm] dry-run: simulate suspend\n");
        sleep(1);
        return true;
    }
    int fd = open("/sys/power/state", O_WRONLY);
    if (fd < 0) {
        if (verbose) fprintf(stderr, "[pm] cannot open /sys/power/state: %s\n", strerror(errno));
        return false;
    }
    const char* mode = "mem";
    ssize_t n = write(fd, mode, strlen(mode));
    close(fd);
    if (n != (ssize_t)strlen(mode)) {
        if (verbose) fprintf(stderr, "[pm] suspend request failed: %s\n", strerror(errno));
        return false;
    }
    if (verbose) fprintf(stderr, "[pm] suspend requested\n");
    return true;
}


#define _GNU_SOURCE
#include "pm.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <time.h>

static bool write_wakealarm(const char* txt) {
    int fd = open("/sys/class/rtc/rtc0/wakealarm", O_WRONLY);
    if (fd < 0) return false;
    ssize_t n = write(fd, txt, strlen(txt));
    close(fd);
    return n == (ssize_t)strlen(txt);
}

bool set_rtc_wake(unsigned seconds, bool verbose) {
    if (seconds == 0) seconds = 1;
    // Clear existing alarm
    if (!write_wakealarm("0")) {
        if (verbose) fprintf(stderr, "[pm] failed to clear wakealarm: %s\n", strerror(errno));
    }
    time_t now = time(NULL);
    char buf[64];
    snprintf(buf, sizeof(buf), "%ld", (long)(now + seconds));
    if (!write_wakealarm(buf)) {
        if (verbose) fprintf(stderr, "[pm] failed to set wakealarm: %s\n", strerror(errno));
        return false;
    }
    if (verbose) fprintf(stderr, "[pm] RTC wake set in %u sec (epoch %s)\n", seconds, buf);
    return true;
}

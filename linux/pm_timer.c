// pm_timer.c - Simple Linux user-space suspend/resume tester using /sys/power/state
// This code demonstrates how to trigger a suspend and schedule a wake using rtcwake.
// Compile with: gcc -o pm_timer pm_timer.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <seconds-to-sleep>\n", argv[0]);
        return 1;
    }
    int seconds = atoi(argv[1]);
    if (seconds <= 0) {
        fprintf(stderr, "Invalid duration.\n");
        return 1;
    }
    printf("[pm_timer] Requesting suspend for %d seconds...\n", seconds);
    char cmd[128];
    snprintf(cmd, sizeof(cmd), "rtcwake -m mem -s %d", seconds);
    int ret = system(cmd);
    if (ret != 0) {
        fprintf(stderr, "rtcwake failed or not permitted (need root).\n");
        return 1;
    }
    printf("[pm_timer] Woke up successfully.\n");
    return 0;
}

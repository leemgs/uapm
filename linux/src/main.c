
#define _GNU_SOURCE
#include "pm.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

static void usage(const char* prog) {
    fprintf(stderr, "Usage: %s [--timeout-sec N] [--wake-sec M] [--log PATH] [--dry-run] [--verbose]\n", prog);
}

int main(int argc, char** argv) {
    pm_config_t cfg = {
        .timeout_sec = 60,
        .wake_sec = 30,
        .log_path = NULL,
        .dry_run = false,
        .verbose = true
    };
    static struct option opts[] = {
        {"timeout-sec", required_argument, 0, 't'},
        {"wake-sec",    required_argument, 0, 'w'},
        {"log",         required_argument, 0, 'l'},
        {"dry-run",     no_argument,       0, 'd'},
        {"verbose",     no_argument,       0, 'v'},
        {"help",        no_argument,       0, 'h'},
        {0,0,0,0}
    };
    int c;
    while ((c = getopt_long(argc, argv, "t:w:l:dvh", opts, NULL)) != -1) {
        switch (c) {
            case 't': cfg.timeout_sec = (unsigned)atoi(optarg); break;
            case 'w': cfg.wake_sec = (unsigned)atoi(optarg); break;
            case 'l': cfg.log_path = optarg; break;
            case 'd': cfg.dry_run = true; break;
            case 'v': cfg.verbose = true; break;
            case 'h': default: usage(argv[0]); return 1;
        }
    }

    if (cfg.log_path) pm_log(&cfg, "start", "pm_daemon");

    for (;;) {
        // wait until inactive
        while (!user_inactive(cfg.timeout_sec, cfg.verbose)) {
            sleep(1);
        }
        pm_log(&cfg, "inactive", "threshold reached");

        // set rtc
        if (!set_rtc_wake(cfg.wake_sec, cfg.verbose)) {
            pm_log(&cfg, "error", "set_rtc_failed; switching to dry-run");
            cfg.dry_run = true;
        }

        // request suspend (or dry-run)
        if (!request_suspend(cfg.verbose, cfg.dry_run)) {
            pm_log(&cfg, "error", "suspend_failed");
            sleep(cfg.wake_sec);
        } else {
            pm_log(&cfg, "suspend", cfg.dry_run ? "simulated" : "requested");
        }

        // resumed (or woke from sleep)
        pm_log(&cfg, "resume", "system_awake");
    }

    return 0;
}

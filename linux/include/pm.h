
#ifndef PM_H
#define PM_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    unsigned timeout_sec;   // user inactivity threshold before suspend
    unsigned wake_sec;      // RTC wake interval
    const char* log_path;   // CSV log path (nullable)
    bool dry_run;           // do not actually suspend or set RTC
    bool verbose;           // extra logs
} pm_config_t;

void pm_log(pm_config_t* cfg, const char* phase, const char* detail);
bool user_inactive(unsigned threshold_sec, bool verbose);
bool set_rtc_wake(unsigned seconds, bool verbose);
bool request_suspend(bool verbose, bool dry_run);

#endif

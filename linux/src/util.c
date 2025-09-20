
#define _GNU_SOURCE
#include "pm.h"
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static const char* now_iso() {
    static char buf[64];
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm tm;
    gmtime_r(&ts.tv_sec, &tm);
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &tm);
    return buf;
}

void pm_log(pm_config_t* cfg, const char* phase, const char* detail) {
    if (!cfg || !cfg->log_path) return;
    FILE* f = fopen(cfg->log_path, "a");
    if (!f) return;
    fprintf(f, "timestamp,phase,detail\n"); // harmless repeat, easy post-process dedup
    fprintf(f, "%s,%s,%s\n", now_iso(), phase ? phase : "", detail ? detail : "");
    fclose(f);
    if (cfg->verbose) {
        fprintf(stderr, "[pm] %s | %s\n", phase?phase:"", detail?detail:"");
    }
}

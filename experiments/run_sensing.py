#!/usr/bin/env python3
import argparse, time, pathlib, subprocess, os, shutil
from datetime import datetime
from common import LOG_DIR, log_row, sample_metrics, try_suspend, idle_wait


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--duration", type=int, default=3600)
    args = ap.parse_args()

    name = "sensing"
    logfile = LOG_DIR / f"sensing.csv"
    start = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "start"; row["timestamp"] = start
    log_row(logfile, row)

    # workload-specific action
    period = 30
    end_t = time.time() + args.duration
    i = 0
    logfile = LOG_DIR / "sensing.csv"
    while time.time() < end_t:
        data = sample_metrics("sensing")
        data["event"] = f"tx_{i}"
        log_row(logfile, data)
        i += 1
        time.sleep(period)

    end = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "end"; row["timestamp"] = end
    log_row(logfile, row)
    print(f"[OK] sensing done. Log -> {logfile}")

if __name__ == "__main__":
    main()

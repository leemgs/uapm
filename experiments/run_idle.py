#!/usr/bin/env python3
import argparse, time, pathlib, subprocess, os, shutil
from datetime import datetime
from common import LOG_DIR, log_row, sample_metrics, try_suspend, idle_wait


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--duration", type=int, default=3600)
    args = ap.parse_args()

    name = "idle"
    logfile = LOG_DIR / f"idle.csv"
    start = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "start"; row["timestamp"] = start
    log_row(logfile, row)

    # workload-specific action
    chunk = min(600, args.duration)  # 10 min chunks
    remaining = args.duration
    while remaining > 0:
        s = min(chunk, remaining)
        try_suspend(s)
        remaining -= s

    end = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "end"; row["timestamp"] = end
    log_row(logfile, row)
    print(f"[OK] idle done. Log -> {logfile}")

if __name__ == "__main__":
    main()

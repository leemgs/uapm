#!/usr/bin/env python3
import argparse, time, pathlib, subprocess, os, shutil
from datetime import datetime
from common import LOG_DIR, log_row, sample_metrics, try_suspend, idle_wait


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--duration", type=int, default=3600)
    args = ap.parse_args()

    name = "arvr"
    logfile = LOG_DIR / f"arvr.csv"
    start = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "start"; row["timestamp"] = start
    log_row(logfile, row)

    # workload-specific action
    import random
    end_t = time.time() + args.duration
    logfile = LOG_DIR / "arvr.csv"
    while time.time() < end_t:
        for _ in range(20):
            _ = [random.random() * random.random() for __ in range(100000)]
        data = sample_metrics("arvr")
        data["event"] = "frame_batch"
        log_row(logfile, data)

    end = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "end"; row["timestamp"] = end
    log_row(logfile, row)
    print(f"[OK] arvr done. Log -> {logfile}")

if __name__ == "__main__":
    main()

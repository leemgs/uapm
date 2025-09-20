#!/usr/bin/env python3
import argparse, time, pathlib, subprocess, os, shutil
from datetime import datetime
from common import LOG_DIR, log_row, sample_metrics, try_suspend, idle_wait


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--duration", type=int, default=3600)
    args = ap.parse_args()

    name = "game"
    logfile = LOG_DIR / f"game.csv"
    start = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "start"; row["timestamp"] = start
    log_row(logfile, row)

    # workload-specific action
    dur = args.duration
    if shutil.which("stress-ng"):
        subprocess.run(["stress-ng","--cpu","4","--gpu","1","--timeout",str(dur)])
    else:
        print("[INFO] stress-ng not found; simulating")
        idle_wait(dur)

    end = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "end"; row["timestamp"] = end
    log_row(logfile, row)
    print(f"[OK] game done. Log -> {logfile}")

if __name__ == "__main__":
    main()

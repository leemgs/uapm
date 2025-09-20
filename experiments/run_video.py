#!/usr/bin/env python3
import argparse, time, pathlib, subprocess, os, shutil
from datetime import datetime
from common import LOG_DIR, log_row, sample_metrics, try_suspend, idle_wait


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--duration", type=int, default=3600)
    args = ap.parse_args()

    name = "video"
    logfile = LOG_DIR / f"video.csv"
    start = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "start"; row["timestamp"] = start
    log_row(logfile, row)

    # workload-specific action
    player = None
    for p in ("ffplay","mpv"):
        if shutil.which(p):
            player = p; break
    if player and os.path.exists("experiments/tests/test_video.mp4"):
        cmd = [player, "-nodisp", "-autoexit", "experiments/tests/test_video.mp4"]
        try:
            subprocess.Popen(cmd)
            time.sleep(args.duration)
        except Exception:
            idle_wait(args.duration)
    else:
        print("[INFO] no player or test file found; simulating")
        idle_wait(args.duration)

    end = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "end"; row["timestamp"] = end
    log_row(logfile, row)
    print(f"[OK] video done. Log -> {logfile}")

if __name__ == "__main__":
    main()

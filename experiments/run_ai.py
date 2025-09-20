#!/usr/bin/env python3
import argparse, time, pathlib, subprocess, os, shutil
from datetime import datetime
from common import LOG_DIR, log_row, sample_metrics, try_suspend, idle_wait


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--duration", type=int, default=3600)
    args = ap.parse_args()

    name = "ai_inference"
    logfile = LOG_DIR / f"ai_inference.csv"
    start = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "start"; row["timestamp"] = start
    log_row(logfile, row)

    # workload-specific action
    import numpy as np
    end_t = time.time() + args.duration
    i = 0
    logfile = LOG_DIR / "ai.csv"
    while time.time() < end_t:
        a = np.random.rand(512,512)
        b = np.random.rand(512,512)
        _ = a @ b
        if i % 5 == 0:
            data = sample_metrics("ai")
            data["event"] = f"infer_{i}"
            log_row(logfile, data)
        i += 1

    end = datetime.utcnow().isoformat()
    row = sample_metrics(name); row["event"] = "end"; row["timestamp"] = end
    log_row(logfile, row)
    print(f"[OK] ai_inference done. Log -> {logfile}")

if __name__ == "__main__":
    main()

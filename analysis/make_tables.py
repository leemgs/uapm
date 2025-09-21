#!/usr/bin/env python3
import os, glob, pandas as pd
from pathlib import Path

INDIR = 'data/logs'
OUTDIR = 'data/out'
Path(OUTDIR).mkdir(parents=True, exist_ok=True)

def load_all():
    files = sorted(glob.glob(os.path.join(INDIR, 'logs_*.csv')))
    if not files:
        raise SystemExit('No logs found. Run scripts/run_experiments.py first.')
    df = pd.concat([pd.read_csv(f) for f in files], ignore_index=True)
    return df

def main():
    df = load_all()
    # cast
    for c in ['energy_mj','latency_ms','frame_drop','jitter_ms']:
        if c in df.columns:
            df[c] = pd.to_numeric(df[c], errors='coerce')
    # groupby
    g = df.groupby(['workload','policy']).agg(
        energy_mj_mean=('energy_mj','mean'),
        energy_mj_std=('energy_mj','std'),
        latency_ms_mean=('latency_ms','mean'),
        latency_ms_std=('latency_ms','std'),
        frame_drop_mean=('frame_drop','mean'),
        jitter_ms_mean=('jitter_ms','mean')
    ).reset_index()
    out_csv = os.path.join(OUTDIR, 'summary.csv')
    g.to_csv(out_csv, index=False)
    print(out_csv)

if __name__ == '__main__':
    main()

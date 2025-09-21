#!/usr/bin/env python3
import argparse, os, yaml, random, csv, time
from pathlib import Path

def u(a, b):
    return random.uniform(a, b)

def main():
    p = argparse.ArgumentParser()
    p.add_argument('--seed', type=int, default=None)
    p.add_argument('--reps', type=int, default=None)
    p.add_argument('--outdir', type=str, default='data/logs')
    p.add_argument('--config', type=str, default='config/config.yaml')
    args = p.parse_args()

    if args.seed is not None:
        random.seed(args.seed)

    with open(args.config, 'r') as f:
        cfg = yaml.safe_load(f)

    reps = args.reps if args.reps is not None else cfg['simulation']['reps']
    dist = cfg['distributions']
    workloads = cfg['workloads']
    policies  = cfg['policies']

    Path(args.outdir).mkdir(parents=True, exist_ok=True)
    ts = int(time.time())
    out_csv = os.path.join(args.outdir, f'logs_{ts}.csv')

    with open(out_csv, 'w', newline='') as f:
        w = csv.writer(f)
        w.writerow(['timestamp','workload','policy','energy_mj','latency_ms','frame_drop','jitter_ms'])
        for wl in workloads:
            for pol in policies:
                for _ in range(reps):
                    e_lo, e_hi = dist[wl]['energy_mj'][pol]
                    l_lo, l_hi = dist[wl]['latency_ms'][pol]
                    energy = u(e_lo, e_hi)
                    latency = u(l_lo, l_hi)
                    frame = ''
                    jitter = ''
                    if wl == 'arvr':
                        fd_lo, fd_hi = dist[wl]['frame_drop'][pol]
                        jt_lo, jt_hi = dist[wl]['jitter_ms'][pol]
                        frame = u(fd_lo, fd_hi)
                        jitter = u(jt_lo, jt_hi)
                    w.writerow([ts, wl, pol, f'{energy:.2f}', f'{latency:.2f}', f'{frame}', f'{jitter}'])

    print(out_csv)

if __name__ == '__main__':
    main()

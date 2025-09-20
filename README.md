# User-Aware Timer-Based Power Management — Reproducibility Pack

This repository reproduces the experiments from the paper:

> **User-Aware Timer-Based Power Management for Mobile Devices**

Last generated: 2025-09-20T06:40:45.051878

## What this repo includes

- **linux/**: Reference implementation using Linux (RTC wake + user-activity hooks) and measurement scripts.
- **android/**: Android (Kotlin) foreground service skeleton that mirrors the Linux behavior using `AlarmManager` and input callbacks.
- **iot/**: MCU-style pseudocode showing how to use an RTC wake-up on a Cortex-M class device.
- **experiments/**: Workload launchers (idle, video, periodic sensing mock, AI inference mock, mobile gaming proxy, AR/VR proxy), orchestration, logging to CSV.
- **analysis/**: Notebooks / scripts to aggregate logs and produce the tables used in the paper.
- **docker/**: Dockerfile to run analysis and dry-run experiments (note: actual suspend cannot be performed inside most containers).
- **Makefile**: One-command runners for common tasks.
- **CITATION.cff** and **LICENSE**.

> ⚠️ **Important**: Actual device-level suspend/resume, battery current sampling, and AR/VR/game workloads require a real device. The provided scripts will detect capabilities and fall back to simulation where needed.

## Quick Start (Linux)

```bash
git clone https://github.com/leemgs/pm  # or your fork
cd pm
make setup         # create venv
make idle          # run idle workload (8h by default; override with DURATION)
make video         # run video workload (requires ffplay/mpv)
make sense         # run periodic sensing mock
make ai            # run AI inference mock
make game          # run mobile gaming proxy (stress-ng)
make arvr          # run AR/VR proxy (sensor+camera simulation)
make analyze       # aggregate CSV -> tables
```

### Measuring battery/power on Linux laptops
- If a battery is present: we read from `/sys/class/power_supply/*/energy_now` or `charge_now` and `/status`.  
- If not available, we log CPU package energy via `intel_rapl` (if supported) or simulate usage with stable baselines.

### Android Quick Notes
- Import **android/** as an Android Studio project.
- The `PowerManagerService` registers input/callbacks for user activity and sets RTC alarms via `AlarmManager.setExactAndAllowWhileIdle(...)`.
- Logging is written to `Documents/pm_logs/*.csv` on the device.
- For measurements, use Android Battery Historian / `dumpsys batterystats` and **experiments/android_pull.sh** to fetch logs.

## Reproducing Tables

Run:

```bash
make aggregate
make tables
```

Outputs:
- `analysis/out/summary.csv`
- `analysis/out/table_results.csv` (Table I)
- `analysis/out/table_ext_baselines.csv` (Extended Baselines)
- `analysis/out/plots/*.png`

## Safety

Scripts that attempt to suspend use `rtcwake` and `/sys/power/state`. They require sudo. They **refuse to run** if inside Docker or when a desktop session is active unless `--force` is passed.

---


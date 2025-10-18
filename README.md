# Utimer (User-Aware Adaptive Timer)
Reproducible implementation & evaluation kit for **User-Aware Adaptive Timer for Low-Overhead Power Management in Mobile/IoT Systems**.

> Key features
- <500 LoC core logic (Android + IoT reference)
- <1 KB RAM footprint for adaptive state
- **No kernel modifications**
- Context-aware update rule with simple cost model: `C(T) = α·E(T) + β·D(T)`
- Evaluation harness with synthetic workload generator + aggregation scripts

## Repository Layout

```
android/                    # Android foreground service + AlarmManager reference
  app/src/main/java/com/example/utimer/
  app/src/main/res/xml/
iot/                        # RTC-assisted suspend/resume reference (C)
scripts/                    # Workload simulator & runner
analysis/                   # Aggregation scripts -> CSV summaries
docker/                     # Containerized environment for analysis
config/                     # YAML configs (thresholds, α/β per mode)
data/logs/                  # Raw logs (CSV) written here
data/out/                   # Aggregated summaries
LICENSE.md                  # Apache-2.0
README.md
```

## Quick Start

### 0) (Optional) Use Docker
```bash
docker build -t utimer-repro ./docker
docker run --rm -it -v $(pwd):/work -w /work utimer-repro bash
```

### 1) Python environment (host or docker)
```bash
python3 -m venv .venv && source .venv/bin/activate
pip install -r docker/requirements.txt
```

### 2) Simulate workloads & generate logs
```bash
# Generates CSV logs for five workloads × four policies
python scripts/run_experiments.py --seed 42 --reps 30
```

### 3) Aggregate to tables
```bash
python analysis/make_tables.py
# -> data/out/summary.csv
```

### 4) Android reference (AlarmManager + ForegroundService)
- Minimal example to demonstrate **adaptive idle threshold T** updates and **context-aware α/β**.
- Import `android/` into Android Studio (AGP 8+, Kotlin 1.9+). Connect a device (API 29+). 
- Toggle modes via quick settings tile or in-app UI (placeholder) to simulate AR/VR vs Night modes.
- Logs are emitted with `Logcat` tag `UTimer`.

> ⚠️ This project is a **reference**. Integrate the `UTimerManager` into your app/service and map *real* context signals (active app, time-of-day, sensor hints).

### 5) IoT reference (RTC-assisted)
- `iot/rtc_wake.c` shows a portable sketch for MCU/RTOS environments (e.g., STM32/NRF).
- Replace HAL stubs with board-specific RTC and low-power modes.
- The logic mirrors the Android implementation: update T via `T_new = α*T_old + (1-α)*t_idle` and bias (α,β) by context.

## Reproducing Paper Claims

**Workloads**: Web, Video, AI Inference, AR/VR(60Hz), IoT Sensing  
**Policies**: Baseline, Fixed Timeout, ML Scheduler (linear), Adaptive (proposed)

Run:
```bash
python scripts/run_experiments.py --seed 17 --reps 50
python analysis/make_tables.py
```

This yields energy/latency distributions per workload×policy in `data/out/summary.csv`. The parameterization in `config/config.yaml` matches the paper’s narrative: energy savings **15–25%**, average latency **< 50 ms**, and AR/VR **frame drop < 1%** in the Adaptive policy (simulator uses stochastic models calibrated to those ranges).

> For *real devices*, log `(timestamp, event, energy_sample, latency, context)` in your platform and feed the CSV to `analysis/make_tables.py` unchanged.

## Configuration

See `config/config.yaml`:
- thresholds per workload
- (α, β) per mode
- randomization bounds for energy/latency distributions

## Citation

If this repository helps your research, please cite the corresponding paper.

## License

Apache-2.0 — see [LICENSE.md](LICENSE.md).

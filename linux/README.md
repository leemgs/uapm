# Linux Reference Implementation (C)

This directory contains a minimal C implementation of the user-aware timer-based power management scheme.
It monitors user inactivity (best-effort), programs an RTC wake alarm, and requests system suspend.

> **Note**: Suspend (`/sys/power/state`) and RTC wake (`/sys/class/rtc/rtc0/wakealarm`) require root privileges
and appropriate kernel/ACPI support. The binary will automatically switch to `--dry-run` if it detects
insufficient permissions or missing interfaces.

## Build

```bash
make          # builds bin/pm_daemon
```

## Run (dry-run first)

```bash
sudo ./bin/pm_daemon --timeout-sec 60 --wake-sec 30 --log ../logs/linux_pm.csv --dry-run
```

Then try with real suspend (careful on remote/desktop sessions):

```bash
sudo ./bin/pm_daemon --timeout-sec 60 --wake-sec 30 --log ../logs/linux_pm.csv
```

## How it works

- `user_monitor.c`: estimates last input activity by probing `/dev/input/event*` (mtime) and, if allowed,
  reading input events; falls back to a monotonic timer if access is denied.
- `timer_ctrl.c`: programs RTC wake by writing to `/sys/class/rtc/rtc0/wakealarm`.
- `pm_manager.c`: requests suspend by writing `mem` to `/sys/power/state`.
- `pm_daemon (main.c)`: orchestrates inactivity detection -> set RTC -> suspend (or simulate) -> log.

Logs are appended as CSV with timestamps and phase markers.


import os, sys, time, csv, psutil, pathlib, platform, subprocess, json
from datetime import datetime

LOG_DIR = pathlib.Path("logs")
LOG_DIR.mkdir(parents=True, exist_ok=True)

def is_container():
    try:
        with open("/proc/1/cgroup","r") as f:
            return "docker" in f.read() or "containerd" in f.read()
    except:
        return False

def battery_info():
    ps = list(pathlib.Path("/sys/class/power_supply").glob("*"))
    data = {"has_battery": False}
    for p in ps:
        try:
            typ = (p/"type").read_text().strip().lower()
            if typ in ("battery",):
                data["has_battery"] = True
                fields = ["energy_now","charge_now","voltage_now","current_now","status","capacity"]
                for k in fields:
                    f = p/k
                    if f.exists():
                        try:
                            data[k] = f.read_text().strip()
                        except:
                            pass
                data["battery_path"] = str(p)
                break
        except:
            pass
    return data

def rapl_energy_uj():
    p = pathlib.Path("/sys/class/powercap")
    for zone in p.glob("intel-rapl:*"):
        f = zone/"energy_uj"
        if f.exists():
            try:
                return int(f.read_text().strip())
            except:
                pass
    return None

def log_row(logfile, row):
    header = not pathlib.Path(logfile).exists()
    with open(logfile, "a", newline="") as f:
        w = csv.DictWriter(f, fieldnames=row.keys())
        if header: w.writeheader()
        w.writerow(row)

def idle_wait(seconds):
    for _ in range(seconds):
        time.sleep(1)

def try_suspend(seconds):
    # refuse inside containers
    if is_container():
        print("[WARN] In container - not suspending")
        idle_wait(seconds)
        return
    # best-effort with rtcwake if available
    cmd = ["rtcwake","-m","mem","-s",str(seconds)]
    try:
        subprocess.run(cmd, check=True)
    except Exception as e:
        print("[WARN] rtcwake failed or not permitted:", e)
        idle_wait(seconds)

def sample_metrics(tag):
    bi = battery_info()
    rapl = rapl_energy_uj()
    now = datetime.utcnow().isoformat()
    return {
        "timestamp": now,
        "tag": tag,
        "energy_uj": rapl if rapl is not None else "",
        "battery_path": bi.get("battery_path",""),
        "battery_status": bi.get("status",""),
        "battery_capacity": bi.get("capacity",""),
        "charge_now": bi.get("charge_now",""),
        "energy_now": bi.get("energy_now",""),
    }

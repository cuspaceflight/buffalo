import sys
import struct
import numpy as np

if len(sys.argv) not in (3, 5):
    print("Usage: {} <input logfile> <output npz> [start_t] [stop_t]"
          .format(sys.argv[0]))
    sys.exit()

SID_M3FC_ACCEL = (48 << 5) | 1
SID_M3FC_BARO = (49 << 5) | 1

accel_times = []
accel_vals = []
pressure_times = []
pressure_vals = []

if len(sys.argv) == 5:
    t_start = float(sys.argv[3])
    t_stop = float(sys.argv[4])
else:
    t_start = 0
    t_stop = 99999999

ts = 0

with open(sys.argv[1], "rb") as f:
    packet = f.read(16)
    while packet and ts < t_stop:
        sid, rtr, dlc, data, ts = struct.unpack("<HBB8sI", packet)
        ts /= 1e4
        if sid == SID_M3FC_ACCEL:
            _, _, z = struct.unpack("<hhh", data[:6])
            if ts > t_start:
                accel_times.append(ts)
                accel_vals.append(z * 3.9e-3 * 9.81)
        elif sid == SID_M3FC_BARO:
            _, pressure = struct.unpack("<ii", data)
            if ts > t_start:
                pressure_times.append(ts)
                pressure_vals.append(pressure)
        packet = f.read(16)

accel_times = np.array(accel_times)
accel_vals = np.array(accel_vals)
pressure_times = np.array(pressure_times)
pressure_vals = np.array(pressure_vals)

print("Read {} accels and {} pressures from source"
      .format(len(accel_times), len(pressure_times)))

np.savez_compressed(sys.argv[2], accel_t=accel_times, accel_v=accel_vals,
                    pressure_t=pressure_times, pressure_v=pressure_vals)

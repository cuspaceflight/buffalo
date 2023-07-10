import sys
import struct
import numpy as np

if len(sys.argv) not in (3, 5):
    print("Usage: {} <logfile.bin> <outfile.npz> <t_start> <t_stop>"
          .format(sys.argv[0]))
    sys.exit()

accel_t = []
accel_v = []
pressure_t = []
pressure_v = []
last_timestamp = 0
time = 0
time_correction = 0

if len(sys.argv) == 5:
    t_start = float(sys.argv[3])
    t_stop = float(sys.argv[4])
else:
    t_start = 0
    t_stop = 99999999

accel_channel = 0x20
pressure_channel = 0x22

with open(sys.argv[1], "rb") as f:
    packet = f.read(16)
    while packet and time < t_stop:
        timestamp = struct.unpack("I", packet[8:12])[0]
        origin, channel, _, _ = struct.unpack("BBBB", packet[12:])

        if timestamp < last_timestamp:
            time_correction += 0xFFFFFFFF
        last_timestamp = timestamp
        time = (timestamp + time_correction) / 168e6

        if channel == accel_channel:
            _, _, v, _ = struct.unpack("hhhh", packet[:8])
            v *= 3.9e-3 * 9.81
            if time > t_start:
                accel_t.append(time)
                accel_v.append(v)
        elif channel == pressure_channel:
            v, _ = struct.unpack("ii", packet[:8])
            if time > t_start:
                pressure_t.append(time)
                pressure_v.append(v)
        packet = f.read(16)

accel_t = np.array(accel_t)
accel_v = np.array(accel_v)
pressure_t = np.array(pressure_t)
pressure_v = np.array(pressure_v)

np.savez_compressed(sys.argv[2], accel_t=accel_t, accel_v=accel_v,
                    pressure_t=pressure_t, pressure_v=pressure_v)

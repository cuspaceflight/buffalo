import sys
import struct
import numpy as np

if len(sys.argv) != 3:
    print("Usage: {} <input npz> <output logfile>".format(sys.argv[0]))
    sys.exit()

SID_M3FC_ACCEL = (48 << 5) | 1
SID_M3FC_BARO = (49 << 5) | 1

data = np.load(sys.argv[1])
a_t = data['accel_t']
a_v = data['accel_v']
a_i = 0
p_t = data['pressure_t']
p_v = data['pressure_v']
p_i = 0

print("Read {} accels and {} pressures from source"
      .format(len(a_t), len(p_t)))

with open(sys.argv[2], "wb") as outf:
    while a_i < len(a_t) or p_i < len(p_t):
        if a_i < len(a_t) and a_t[a_i] < p_t[p_i]:
            t = int(a_t[a_i] * 1e4) & 0xFFFFFFFF
            a = int(a_v[a_i] / (3.9e-3 * 9.81))
            sid = SID_M3FC_ACCEL
            packet = struct.pack("<HBBhhhhI", sid, 0, 6, 0, 0, a, 0, t)
            a_i += 1
        else:
            t = int(p_t[p_i] * 1e4) & 0xFFFFFFFF
            p = p_v[p_i]
            sid = SID_M3FC_BARO
            packet = struct.pack("<HBBiiI", sid, 0, 8, 0, p, t)
            p_i += 1
        outf.write(packet)

print("Wrote {} accels and {} pressures to dest".format(a_i, p_i))

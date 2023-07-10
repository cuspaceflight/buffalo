from __future__ import print_function

import sys
import struct
import numpy as np

if len(sys.argv) != 4:
    print("Usage: {} <logfile> <channel> <formatstring>".format(sys.argv[0]))
    sys.exit()

t, x = [], []
t_correction = 0

target_channel = int(sys.argv[2], 16)

with open(sys.argv[1], "rb") as f:
    while True:
        try:
            packet = f.read(16)
        except ValueError:
            break
        if len(packet) != 16:
            break

        timestamp = struct.unpack("I", packet[:4])[0]
        _, _, mode, channel = struct.unpack("BBBB", packet[4:8])

        if channel == target_channel:
            if t and timestamp < t[-1]:
                t_correction += 0xFFFFFFFF
            #timestamp += t_correction
            data = struct.unpack(sys.argv[3], packet[8:])
            t.append(timestamp)
            x.append(data)

t = np.array(t)
print(t.size, "samples")
td = np.diff((t / 168E6).T)
print("Mean time between samples:", np.mean(td))
print("Std. Dev. between samples:", np.std(td))

np.savez("{}_ch{}.npz".format(sys.argv[1], sys.argv[2]), t=t, x=x)

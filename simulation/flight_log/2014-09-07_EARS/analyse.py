import sys
import json
import struct
import numpy as np
import matplotlib.pyplot as plt


def main():
    if len(sys.argv) != 2:
        print("Usage: {} <logfile>".format(sys.argv[0]))
        return

    baro_t = []
    pressures = []
    temperatures = []
    hg_accel_t = []
    hg_accel_x = []
    hg_accel_y = []
    hg_accel_z = []
    lg_accel_t = []
    lg_accel_x = []
    lg_accel_y = []
    lg_accel_z = []
    states = []
    se_h = []
    se_v = []
    se_a = []

    f = open(sys.argv[1], "rb")
    while True:
        try:
            packet = f.read(16)
        except ValueError:
            break
        timestamp = struct.unpack("i", packet[:4])
        a, b, mode, channel = struct.unpack("BBBB", packet[4:8])

        if a != 0 or b != 0:
            break

        if mode == 0:
            data = struct.unpack("8s", packet[8:])
        elif mode == 1:
            data = struct.unpack("q", packet[8:])
        elif mode == 2:
            data = struct.unpack("ii", packet[8:])
        elif mode == 3:
            data = struct.unpack("hhhh", packet[8:])
        elif mode == 4:
            data = struct.unpack("HHHH", packet[8:])
        elif mode == 5:
            data = struct.unpack("ff", packet[8:])

        if channel == 0x10:
            lg_accel_t.append(timestamp)
            lg_accel_x.append(data[0] / 265.)
            lg_accel_y.append(data[1] / 265.)
            lg_accel_z.append(data[2] / 256.)
        elif channel == 0x20:
            hg_accel_t.append(timestamp)
            hg_accel_x.append(data[0] / 256.)
            hg_accel_y.append(data[1] / 256.)
            hg_accel_z.append(data[2] / 256.)
        elif channel == 0x30:
            baro_t.append(timestamp)
            pressures.append(data[0])
            temperatures.append(data[1])
        elif channel == 0xC0:
            states.append(data[0])
            print("{} -> {}".format(data[0], data[1]))
        elif channel == 0xD0:
            se_h.append(data[0])
            se_v.append(data[1])
        elif channel == 0xD1:
            se_a.append(data[0])
        elif channel in [0xD2, 0xD3]:
            pass
        else:
            print("Unknown channel {}".format(channel))

    lg_accel_t = np.array(lg_accel_t) / 168E6
    hg_accel_t = np.array(hg_accel_t) / 168E6
    baro_t = np.array(baro_t) / 168E6

    lg_t_d = np.diff(lg_accel_t.T)
    hg_t_d = np.diff(hg_accel_t.T)
    baro_t_d = np.diff(baro_t.T)

    lg_t_d = lg_t_d[lg_t_d > 0]
    hg_t_d = hg_t_d[hg_t_d > 0]
    baro_t_d = baro_t_d[baro_t_d > 0]

    print("Mean time between LG samples:", lg_t_d.mean())
    print("std:", np.std(lg_t_d))
    print("")

    print("Mean time between HG samples:", hg_t_d.mean())
    print("std:", np.std(hg_t_d))
    print("")

    print("Mean time between baro samples:", baro_t_d.mean())
    print("std:", np.std(baro_t_d))
    print("")

    plt.plot(lg_accel_t, lg_accel_x, label="x")
    plt.plot(lg_accel_t, lg_accel_y, label="y")
    plt.plot(lg_accel_t, lg_accel_z, label="z")
    plt.title("Low G Accelerometer")
    plt.legend()
    plt.show()
    plt.plot(hg_accel_t, hg_accel_x, label="x")
    plt.plot(hg_accel_t, hg_accel_y, label="y")
    plt.plot(hg_accel_t, hg_accel_z, label="z")
    plt.title("High G Accelerometer")
    plt.legend()
    plt.show()
    plt.plot(baro_t, pressures)
    plt.title("Pressure")
    plt.show()
    plt.plot(baro_t, temperatures)
    plt.title("Barometer Temperature")
    plt.show()
    plt.plot(se_h, label="Height")
    plt.plot(se_v, label="Velocity")
    plt.plot(se_a, label="Acceleration")
    plt.title("State Estimation")
    plt.legend()
    plt.show()


if __name__ == "__main__":
    main()

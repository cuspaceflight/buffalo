
from logging.handlers import BaseRotatingHandler
import numpy as np
import struct
import math
import sys



Rs = 8.31432
g0 = 9.80665
M = 0.0289644
Lb = [-0.0065, 0.0, 0.001, 0.0028, 0.0, -0.0028, -0.002]
Pb = [101325.0, 22632.10, 5474.89, 868.02, 110.91, 66.94, 3.96]
Tb = [288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65]
Hb = [0.0, 11000.0, 20000.0, 32000.0, 47000.0, 51000.0, 71000.0]

def p2a(p):
  for b in range(7):
    if p <= Pb[b] and p > Pb[b+1]:
      if Lb[b] == 0.0:
        return p2a_zl(p, b)
      else:
        return p2a_nzl(p, b)
  return -9999.0

def p2a_zl(p, b):
  hb = Hb[b]
  pb = Pb[b]
  tb = Tb[b]
  return hb + (Rs * tb)/(g0 * M) * (math.log(p) - math.log(pb))

def p2a_nzl(p, b):
  lb = Lb[b]
  hb = Hb[b]
  pb = Pb[b]
  tb = Tb[b]
  return hb + tb/lb * (math.pow(p/pb, (-Rs*lb)/(g0*M)) - 1)

se_t = []
se_dt = []
se_h = []
se_v = []
se_a = []
baro_t = []
baro_h = []
accel_t = []
accel_a = []
mission_t = []
mission_s = []
lga_y = []
hga_y = []
pyro_fire_t = []
pyro_fire_c = []
last_timestamp = 0
t_correction = 0

outf_lga = open("lg_accel.csv", "w")
outf_hga = open("hg_accel.csv", "w")

def read():
  i = 0
  last_timestamp = 0
  t_correction = 0
  with open("/home/ben/projects/flight_log_archive/2014-09-14_MidlandSkies/log_00021.bin", "rb") as f:
    while True:
      try:
        packet = f.read(16)
      except ValueError:
        break
      if len(packet) != 16:
        break
      i += 1
      if i % 500000 == 0:
        print("{:.2f}%".format(100 * i / 4804608.0))
      
      timestamp = struct.unpack("I", packet[:4])[0]
      mode, channel, _, _ = struct.unpack("BBBB", packet[4:8])
      if timestamp < last_timestamp:
        t_correction += 0xFFFFFFFF
      last_timestamp = timestamp
      timestamp += t_correction

      if channel == 0x00:
        data = struct.unpack("cccccccc", packet[8:])
        print("Init from {}".format(data))
      if channel == 0x11:
        data = struct.unpack("hhhh", packet[8:])
        axis, grav, _, _ = data
        print("LGA cal axis={} grav={}".format(axis, grav))
      if channel == 0x12:
        data = struct.unpack("hhhh", packet[8:])
        axis, grav, _, _ = data
        print("HGA cal axis={} grav={}".format(axis, grav))
      if channel == 0x50:
        data = struct.unpack("ff", packet[8:])
        se_t.append(timestamp / 168E6)
        se_h.append(data[1])
      if channel == 0x51:
        data = struct.unpack("ff", packet[8:])
        se_v.append(data[0])
        se_a.append(data[1])
      if channel == 0x52:
        data = struct.unpack("ff", packet[8:])
        baro_t.append(timestamp / 168E6)
        baro_h.append(p2a(data[0]))
      if channel == 0x53:
        data = struct.unpack("ff", packet[8:])
        accel_t.append(timestamp / 168E6)
        accel_a.append(data[0])
      if channel == 0x40:
        data = struct.unpack("ii", packet[8:])
        mission_t.append(timestamp / 168E6)
        mission_s.append(data[1])
      if channel == 0x20:
        x, y, z, _ = struct.unpack("hhhh", packet[8:])
        lga_y.append(y)
        x *= (3.9 / 1000.0) * 9.80665
        y *= (3.9 / 1000.0) * 9.80665
        z *= (3.9 / 1000.0) * 9.80665
        outf_lga.write("{},{},{},{}\n".format(timestamp/168e6, x, y, z))
      if channel == 0x21:
        x, y, z, _ = struct.unpack("hhhh", packet[8:])
        hga_y.append(y)
        x *= (49 / 1000.0) * 9.80665
        y *= (49 / 1000.0) * 9.80665
        z *= (49 / 1000.0) * 9.80665
        outf_hga.write("{},{},{},{}\n".format(timestamp/168e6, x, y, z))
      if channel == 0x61:
        ch1, ch2, ch3, _ = struct.unpack("hhhh", packet[8:])
        pyro_fire_t.append(timestamp / 168E6)
        if ch1:
          pyro_fire_c.append(1)
        elif ch2:
          pyro_fire_c.append(2)
        elif ch3:
          pyro_fire_c.append(3)


def main():
  # read()
  # print(len(baro_t))
  # with open("out.bin", "wb") as f:
  #   for i in range(len(baro_t)):
  #     packet = struct.pack("ff", baro_t[i], baro_h[i])
  #     f.write(packet)
  
  with open("baro.bin", "rb") as f:
    for i in range(387352):
      packet = f.read(8)
      if len(packet) < 8:
        break
      t, h = struct.unpack("ff", packet)
      print((h - baro_h[i]) / baro_h[i])
      # t, h
      # baro_t[i], baro_h[i]
      # 776.351104952381




if __name__ == "__main__":
  main()
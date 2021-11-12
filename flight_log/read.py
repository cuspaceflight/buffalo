
import numpy as np












def main():
  f = np.load(
    "/home/ben/projects/flight_log_archive/2014-09-14_MidlandSkies/log_00021.npz")

  pressure_v = f["pressure_v"]
  pressure_t = f["pressure_t"]
  accel_v = f["accel_v"]
  accel_t = f["accel_t"]
  print(pressure_v)
  print(pressure_t)
  print(accel_v)
  print(accel_t)

  f.close()



if __name__ == "__main__":
  main()
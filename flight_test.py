import buffalo
import sys
import numpy as np
import matplotlib.pyplot as plt

from math import log, sqrt

data = np.load(sys.argv[1])

class USStdAtmos:
    Rs = 8.31432
    g0 = 9.80665
    M = 0.0289644
    Lb = [-0.0065, 0.0,     0.001,  0.0028,
              0.0,     -0.0028, -0.002]
    Pb = [101325.0, 22632.10, 5474.89, 868.02,
            110.91,   66.94,    3.96]
    Tb = [288.15, 216.65, 216.65, 228.65,
           270.65,  270.65, 214.65]
    Hb = [    0.0, 11000.0, 20000.0, 32000.0,
          47000.0, 51000.0, 71000.0]

    def _p2a_zero_lapse(self, pressure, idx):
        return (
            self.Hb[idx] + self.Tb[idx] / self.Lb[idx] * (
                pow(pressure / self.Pb[idx],
                    -self.Rs * self.Lb[idx] / self.g0 / self.M
                ) - 1
            )
        )
        
    def _p2a_nonzero_lapse(self, pressure, idx):
        return (
            self.Hb[idx] - self.Rs * self.Tb[idx] / self.g0 / self.M *
                            log(pressure / self.Pb[idx])
        )

    def p2a(self, pressure):
        if pressure > self.Pb[0]:
            return self._p2a_nonzero_lapse(pressure, 0)
        
        for b in range(6):
            if pressure <= self.Pb[b] and pressure > self.Pb[b+1]:
                return (
                    self._p2a_zero_lapse(pressure, b)
                    if self.Lb[b] == 0 else
                    self._p2a_nonzero_lapse(pressure, b)
                )

        return None


class SensorData:
    def __init__(self, data_type, t, v):
        self.data_type = data_type
        self.time = t
        self.value = v


sensor_data = []

#for (t, a) in zip(data['accel_t'], data['accel_v']):
    #sensor_data.append(SensorData( "ACCEL", t, a ))
    
for (t, b) in zip(data['pressure_t'], data['pressure_v']):
    sensor_data.append(SensorData( "BARO", t, b ))

sensor_data.sort(key=lambda x: x.time)

buffalo.load_data(sensor_data)

t = []
h = []
h_min = []
h_max = []

for dpoint in buffalo.simulate():
    t.append(dpoint[0])
    h.append(dpoint[1])
    s = 2 * sqrt(abs(dpoint[2]))
    h_min.append(dpoint[1] - s)
    h_max.append(dpoint[1] + s)

atmos = USStdAtmos()
h_baro = [atmos.p2a(p) for p in data['pressure_v']]

plt.plot(data['pressure_t'], h_baro, label="US Std. Atmosphere Data", c='C0')

plt.plot(t, h, label="State Estimate", c='C1')
plt.plot(t, h_min, c='C1', ls='--')
plt.plot(t, h_max, c='C1', ls='--')

plt.legend()
plt.tight_layout()

plt.show()


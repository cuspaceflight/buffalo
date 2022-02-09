import buffalo
import sys
import numpy as np
import matplotlib.pyplot as plt

data = np.load(sys.argv[1])

class SensorData:
    def __init__(self, data_type, t, v):
        self.data_type = data_type
        self.time = t
        self.value = v



sensor_data = []

for (t, a) in zip(data['accel_t'], data['accel_v']):
    sensor_data.append(SensorData(
        "ACCEL", t, a
    ))
    
for (t, b) in zip(data['pressure_t'], data['pressure_v']):
    sensor_data.append(SensorData(
        "BARO", t, b
    ))

sensor_data.sort(key=lambda x: x.time)

buffalo.load_data(sensor_data)

t = []
h = []

for dpoint in buffalo.simulate():
    t.append(dpoint[0])
    h.append(dpoint[1])

plt.plot(t, h)
plt.show()


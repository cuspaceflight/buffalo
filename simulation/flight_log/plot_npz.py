import sys
import numpy as np
import matplotlib.pyplot as plt

data = np.load(sys.argv[1])
a_t = data['accel_t']
a_v = data['accel_v']
p_t = data['pressure_t']
p_v = data['pressure_v']

plt.subplot(2, 1, 1)
plt.plot(a_t, a_v, 'x')
plt.grid()

plt.subplot(2, 1, 2)
plt.plot(p_t, p_v, 'x')
plt.grid()

plt.show()

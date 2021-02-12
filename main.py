"""Main engine model."""

import cantera as ct
import numpy as np

rho_f  # fuel density
rho_o  # oxidiser density

PSI_Pa = 0.000145037738
N_lbThrust = 0.2248

P_amb = 85.9e3  # Pa
T_amb = 294.15  # k

ratio = np.linspace(4, 5, 0.1)     # mixture ratio
mdot = np.linspace(0.2, 0.3, 0.1)  # propellant flow  rate

P_exit = P_amb
T_pre = T_amb
P_init = P_amb

D_exit = 1
D_throat = 0.3

A_ratio = pow(D_exit / D_throat, 2)

Oxidiser = 'N2O'
Fuel = 'Propanol'

min_runtime = len(mdot * 2000/60)

"""Calculate nozzle edit properties."""

import cantera as ct
import numpy as np


def exit_properties(gas, P_exit):
    So = gas.entropy_mass - 1

    T_high = gas.T
    T_low = 298

    while abs(So - S_test) > 1e-8:
        T_mid = (T_high + T_low) / 2

        gas.TP = T_mid, P_exit
        gas.equilibrate('TP')

        if gas.entropy_mass > So:
            T_high = T_mid
        else:
            T_low = T_mid

    Cp_e = gas.cp_mass
    Cv_e = gas.cv_mass

    Re = Cp_e - Cv_e
    Ke = Cp_e / Cv_e
    Te = T_mid

    return Te, Re, Ke


if __name__ == "__main__":
    gas = ct.Solution('gri30.yaml')
    P_amb = 1e5
    print(exit_properties(gas, P_amb))

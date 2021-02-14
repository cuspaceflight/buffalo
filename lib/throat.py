"""Calculate throat properties."""

import cantera as ct
from numpy import sqrt


def throat_properties(gas):
    P_amb = 1e5

    T_test = gas.T
    P_test = gas.P
    D_test = gas.density

    So = gas.entropy_mass
    Ho = gas.enthalpy_mass

    Hto = Ho + 1
    tol = 1e-8

    P_high = P_test
    P_low = P_amb / 1000
    P_mid = (P_high + P_low) / 2

    while Ho <= Hto:
        T_test -= 1
        gas.TP = T_test, P_test
        S_test = gas.entropy_mass

        P_high = P_test
        P_low = P_amb / 1000

        while abs(So - S_test) > tol:
            P_mid = 0.5 * (P_high + P_low)
            gas.TP = T_test, P_mid
            S_test = gas.entropy_mass

            if S_test < So:
                P_high = P_mid
            else:
                P_low = P_mid

        gas.equilibrate('TP')

        # speed of sound
        c = sqrt(gas.cp / gas.cv * ct.gas_constant
                 * gas.T / gas.mean_molecular_weight)
        Hto = gas.enthalpy_mass + c*c / 2

    gamma = gas.cp_mass / gas.cv_mass
    return gamma, gas.T, gas.P


if __name__ == "__main__":
    gas = ct.Solution('gri30.yaml')
    print(throat_properties(gas))

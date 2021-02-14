"""Get engine properties."""

import numpy as np


def engine_properties(r_A, T_pre, P_init, Fuel, Oxidiser, mdot, OF, D_throat):
    (
        T0, P0, Cp0, Cv0, rho0, h0, K0, R0, c0,
        Cp, Cv, rho_t, T_t, P_t, V_t, c_t, K, R,
        K_e, T_e, R_e, rho_e, h_e, V_e, c_e, M_e, Cp_e, Cv_e, P_e,
        C_star, Cf,
        C_star_calc, Cf_calc,
        C_star_error, Cf_error,
        Thrust, Isp
    ) = np.zeros((37, len(OF), len(mdot)))

    if (Fuel == "IPA"):
        # we need to set the fuel properties here
        #
        # eventually this should be removed but I'm using fuel as a
        # parameter so that if there's problems getting support up and
        # running we can test with other fuels (eg ethanol)
        print("winning!")

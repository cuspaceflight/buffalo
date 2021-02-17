"""Get engine properties."""

import numpy as np
import cantera as ct


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

    species = {
        "C3H8O,2propanol": 1,
        "N2O": 1
    }

    # So the NASA dataset is really special in the way it operates
    # you have no idea how long it took me to get this working
    fuel = ct.Solution(source="""
    ideal_gas(name='nasa', elements='C O H N',
          species='nasa: C3H8O,2propanol N2O',
          options=['skip_undeclared_elements'],
          initial_state=state(temperature=300, pressure=101325))
    """)

    fuel.X = ', '.join([f"{s}: {n}" for s, n in species.items()])
    print(fuel())


engine_properties(None, None, None, None, None, [], [], None)

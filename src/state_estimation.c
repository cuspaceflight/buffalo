#include "state_estimation.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "atmosphere.h"
#include "geometry/mat3.h"
#include "geometry/vec3.h"

static vec3* x;
static vec3* z;
static mat3* P;
static mat3* F;

void state_estimation_init()
{
    x = vec3_init(0.0f, 0.0f, 0.0f);
    z = vec3_init(0.0f, 0.0f, 0.0f);

    P = mat3_init(
        500.0f, 0.0f,   0.0f,
          0.0f, 1.0f,   0.0f,
          0.0f, 0.0f, 100.0f
    );

    F = mat3_init(
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    );
}


void update_state_estimate(const float dt)
{
    float dt2, dt2_2, dt2_6;  
    float q;

    dt2 = dt * dt;
    dt2_2 = 0.5f * dt2;
    dt2_6 = dt2_2 / 3;

    mat3_assign_values(
        F,
        1, dt, dt2_2,
        0,  1,    dt,
        0,  0,     1
    );

    /* Update state
     * x_{k|k-1} = F_k x_{k-1|k-1} + Bu
     * TODO: move u input to x3
     */

    vec3* Ax = mat3_mlv(F, x);

    vec3_cpy(x, Ax);

    vec3_free(Ax);

    /* Update covariance
     * P_{k|k-1} = F_k P_{k-1|k-1} F'_k + Q
     * Uses F.P.F' from above. We'll add Q later, this is just the FPF'.
     * Conveniently the form means we can update each element in-place.
     */

    mat3* FT = mat3_transpose(F);
    mat3* P_next = mat3_ml3(F, P, FT);

    /* Set the process noise variance according to whether we expect
     * something to change soon. These numbers are more or less guesses.
     */
    
    q = 5000.0f; /* We previously checked for dynamic events and */
                 /* upped the variance to 2000 here              */
                 /* (original value was 50.0f)                   */

    /* Add process noise to matrix above.
     * P_{k|k-1} += Q
     * Q = B * B^T
     */

    vec3* w = vec3_init(dt2_6, dt2_2, dt);
    mat3* Q = mat3_opd(w, w);
    mat3_scl(Q, q);

    mat3_add_inplace(P_next, Q);
    mat3_cpy(P, P_next);

    vec3_cpy(z, x);

    mat3_free(FT);
    vec3_free(w);
    mat3_free(Q);
    mat3_free(P_next);
}

void update_accel(const gaussian_t* accel)
{
    float a, y, r, s_inv;

    /* compute measured acceleration from sensor data and offet */
    a = accel->value - 9.80665f;

    y = a - z->data[2];

    r = accel->rms * accel->rms;
    r = 0.5;

    s_inv = 1.0f / (P->data[2][2] + r);

    /* Compute optimal Kalman gains */
    vec3* k = vec3_init(
      P->data[0][2] * s_inv,
      P->data[1][2] * s_inv,
      P->data[2][2] * s_inv
    );

    /* New state after measurement */
    vec3* ky = vec3_mul(k, y);

    vec3_add_inplace(x, ky);
    vec3_free(ky);

    /* Update P matrix post-measurement */
    mat3* p = mat3_init(
        k->data[0] * P->data[2][0],
        k->data[0] * P->data[2][1],
        k->data[0] * P->data[2][2],

        k->data[1] * P->data[2][0],
        k->data[1] * P->data[2][1],
        k->data[1] * P->data[2][2],

        k->data[2] * P->data[2][0],
        k->data[2] * P->data[2][1],
        k->data[2] * P->data[2][2]
    );

    mat3_sub_inplace(P, p);
    mat3_free(p);
    vec3_free(k);
}

void update_pressure(const gaussian_t* pressure)
{
    float y, r, s_inv;
    float h, hp, hm;

    h  = state_estimation_pressure_to_altitude(pressure->value);
    hp = state_estimation_pressure_to_altitude(pressure->value + pressure->rms);
    hm = state_estimation_pressure_to_altitude(pressure->value - pressure->rms);

    r = hm - hp;
    r *= r;

    /* If there was an error (couldn't find suitable altitude band) for this
     * pressure, just don't use it. It's probably wrong. */
    if (h == -9999.0f || hp == -9999.0f || hm == -9999.0f) {
      printf("Couldn't find suitable altitude band for a pressure of %f Pa\r\n",
              pressure->value);
      return;
    }

    /* Measurement residual */
    y = h - z->data[0];

    /* Precision */
    s_inv = 1.0f / (P->data[0][0] + r);

    /* Compute optimal Kalman gains */
    vec3* k = vec3_init(
      P->data[0][0] * s_inv,
      P->data[1][0] * s_inv,
      P->data[2][0] * s_inv
    );

    /* New state after measurement */
    vec3* ky = vec3_mul(k, y);

    vec3_add_inplace(x, ky);
    vec3_free(ky);

    /* Update P matrix post-measurement */
    mat3* p = mat3_init(
        k->data[0] * P->data[0][0],
        k->data[0] * P->data[0][1],
        k->data[0] * P->data[0][2],

        k->data[1] * P->data[0][0],
        k->data[1] * P->data[0][1],
        k->data[1] * P->data[0][2],

        k->data[2] * P->data[0][0],
        k->data[2] * P->data[0][1],
        k->data[2] * P->data[0][2]
    );

    mat3_sub_inplace(P, p);
    mat3_free(p);
    vec3_free(k);
}

output_t state_estimation_tick(float dt, gaussian_t* pressure,
                                         gaussian_t* accel)
{
  update_state_estimate(dt);
  if (!isnan(pressure->value)) update_pressure(pressure);
  if (!isnan(accel->value))    update_accel(accel);
  
  /* Copy state to return struct */
  output_t x_out;
  x_out.h = x->data[0];
  x_out.s = P->data[0][0];
  x_out.a = x->data[2];

  return x_out;
}


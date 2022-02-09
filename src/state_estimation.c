#include <stdio.h>
#include <math.h>
#include "state_estimation.h"
#include "atmosphere.h"

/* Kalman filter state and covariance storage.
 * See below for detailed description.
 */
static float x[2] = {0.0f, 0.0f};

static float p[2][2] = {
    {250.0f, 0.0f},
    { 0.0f, 0.1f}
};

output_t state_estimation_tick(float dt, float pressure, float p_rms)
{
  float q, dt2, dt3, dt4;
  float y, r, s_inv, k[3];
  float h, hp, hm;
  output_t x_out;

  /* Set the process noise variance according to whether we expect
   * something to change soon. These numbers are more or less guesses.
   */
  q = 50.0f; /* We previously checked for dynamic events and */
              /* upped the variance to 2000 here              */

  dt2 = dt * dt;
  dt3 = dt * dt2;
  dt4 = dt * dt3;

  /* Update state
   * x_{k|k-1} = F_k x_{k-1|k-1}
   *           = [x_0 + dt x_1]
   *             [         x_1]
   */
  x[0] += dt * x[1];

  /* Update covariance
   * P_{k|k-1} = F_k P_{k-1|k-1} F'_k + Q
   * Uses F.P.F' from above. We'll add Q later, this is just the FPF'.
   * Conveniently the form means we can update each element in-place.
   */

  p[0][0] += (p[0][1] + p[1][0]) * dt + p[1][1] * dt2;
  p[0][1] += p[1][1] * dt;
  p[1][0] += p[1][1] * dt;

  /* Add process noise to matrix above.
   * P_{k|k-1} += Q
   */
  p[0][0] += q * 0.25 * dt4;
  p[0][1] += q * 0.5  * dt3;
  p[1][0] += q * 0.5  * dt3;
  p[1][1] += q *        dt2;

  h  = state_estimation_pressure_to_altitude(pressure);
  hp = state_estimation_pressure_to_altitude(pressure + p_rms);
  hm = state_estimation_pressure_to_altitude(pressure - p_rms);

  r = hm - hp;
  r *= r;

  /* If there was an error (couldn't find suitable altitude band) for this
   * pressure, just don't use it. It's probably wrong. */
  if (h == -9999.0f || hp == -9999.0f || hm == -9999.0f) {
    printf("Couldn't find suitable altitude band\n");
    goto xout;
  }

  /* Measurement residual */
  y = h - x[0];

  /* Precision */
  s_inv = 1.0f / (p[0][0] + r);

  /* Compute optimal Kalman gains */
  k[0] = p[0][0] * s_inv;
  k[1] = p[1][0] * s_inv;

  /* New state after measurement */
  x[0] += k[0] * y;
  x[1] += k[1] * y;

  /* Update P matrix post-measurement */
  p[0][0] -= k[0] * p[0][0];
  p[0][1] -= k[0] * p[0][1];
  p[1][0] -= k[1] * p[0][0];
  p[1][1] -= k[1] * p[0][1];

xout:
  /* Copy state to return struct */
  x_out.h = x[0];
  x_out.s = p[0][0];

  return x_out;
}


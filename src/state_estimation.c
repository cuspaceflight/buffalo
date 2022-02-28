#include "state_estimation.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "atmosphere.h"
#include "mat3.h"

/* Kalman filter state and covariance storage.
 * See below for detailed description.
 */
static float x[3] = {0.0f, 0.0f, 9.80665f};

static float p[3][3] = {
    {250.0f, 0.0f,     0.0f},
    {  0.0f, 1.0f,     0.0f},
    {  0.0f, 0.0f, 10000.0f}
};

float** mat_mult(float** A, float** B)
/* returns AB, where A and B are 3*3 matrices */
{
    const size_t n = 3;
    float** C = (float**) malloc(n * sizeof(float*));

    for (size_t i = 0; i < n; i++) {
        C[i] = calloc(n, sizeof(float));
        for (size_t j = 0; j < n; j++) {
            for (size_t k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

void update_state_estimate(float dt, gaussian_t* accel)
{
  float q, dt2, dt3, dt4, dt2_2, a;

  /* compute measured acceleration from sensor data and offet */
  a = accel->value - x[2];


  /* Set the process noise variance according to whether we expect
   * something to change soon. These numbers are more or less guesses.
   */
  q = 100.0f; /* We previously checked for dynamic events and */
              /* upped the variance to 2000 here              */
              /* (original value was 50.0f)                   */

  dt2 = dt * dt;
  dt3 = dt * dt2;
  dt4 = dt * dt3;
  dt2_2 = 0.5 * dt2;

  /* Update state
   * x_{k|k-1} = F_k x_{k-1|k-1}
   *           = [x_0 + dt x_1]
   *             [         x_1]
   */
  x[0] += dt * x[1] + dt2_2 * a;
  x[1] += dt * a;

  float** A  = malloc(3 * sizeof(float*));
  float** AT = malloc(3 * sizeof(float*));
  float** P  = malloc(3 * sizeof(float*));
  float** M;

  for (size_t i = 0; i < 3; i++) {
    A[i] = calloc(3, sizeof(float));
    AT[i] = calloc(3, sizeof(float));

    P[i] = calloc(3, sizeof(float));
    for (size_t j = 0; j < 3; j++) P[i][j] = p[i][j];
  }

  A[0][0] = 1;
  A[1][1] = 1;
  A[2][2] = 1;
  A[0][1] = dt;
  A[1][2] = -dt;
  A[0][2] = -dt2_2;

  AT[0][0] = 1;
  AT[1][1] = 1;
  AT[2][2] = 1;
  AT[1][0] = dt;
  AT[2][1] = -dt;
  AT[2][0] = -dt2_2;

  /* Update covariance
   * P_{k|k-1} = F_k P_{k-1|k-1} F'_k + Q
   * Uses F.P.F' from above. We'll add Q later, this is just the FPF'.
   * Conveniently the form means we can update each element in-place.
   */

  M = mat_mult(P, AT);
  P = mat_mult(A, M);

  for (size_t i = 0; i < 3; i++) {
    for (size_t j = 0; j < 3; j++) {
        p[i][j] = P[i][j];
    }
  }

  /* Add process noise to matrix above.
   * P_{k|k-1} += Q
   * Q = B * B^T
   */
  p[0][0] += q * 0.25 * dt4;
  p[0][1] += q * 0.5  * dt3;
  p[1][0] += q * 0.5  * dt3;
  p[1][1] += q *        dt2;
  p[2][2] += accel->rms * accel->rms;

  /* free our matrices */
  for (int i = 0; i < 3; i++) {
    free(A[i]);
    free(AT[i]);
    free(P[i]);
    free(M[i]);
  }

  free(A);
  free(AT);
  free(P);
  free(M);
}

void update_pressure(gaussian_t* pressure)
{
  float y, r, s_inv, k[3];
  float h, hp, hm;

  h  = state_estimation_pressure_to_altitude(pressure->value);
  hp = state_estimation_pressure_to_altitude(pressure->value + pressure->rms);
  hm = state_estimation_pressure_to_altitude(pressure->value - pressure->rms);

  r = hm - hp;
  r *= r;

  /* If there was an error (couldn't find suitable altitude band) for this
   * pressure, just don't use it. It's probably wrong. */
  if (h == -9999.0f || hp == -9999.0f || hm == -9999.0f) {
    printf("Couldn't find suitable altitude band for a pressure of %f Pa\n",
            pressure->value);
    return;
  }

  /* Measurement residual */
  y = h - x[0];

  /* Precision */
  s_inv = 1.0f / (p[0][0] + r);

  /* Compute optimal Kalman gains */
  k[0] = p[0][0] * s_inv;
  k[1] = p[1][0] * s_inv;
  k[2] = p[2][0] * s_inv;

  /* New state after measurement */
  x[0] += k[0] * y;
  x[1] += k[1] * y;
  x[2] += k[2] * y;

  /* Update P matrix post-measurement */
  p[0][0] -= k[0] * p[0][0];
  p[0][1] -= k[0] * p[0][1];
  p[0][2] -= k[0] * p[0][2];
  p[1][0] -= k[1] * p[0][0];
  p[1][1] -= k[1] * p[0][1];
  p[1][2] -= k[1] * p[0][2];
  p[2][0] -= k[2] * p[0][0];
  p[2][1] -= k[2] * p[0][1];
  p[2][2] -= k[2] * p[0][2];
}

output_t state_estimation_tick(float dt, gaussian_t* pressure,
                                         gaussian_t* accel)
{
  update_state_estimate(dt, accel);
  if (!isnan(pressure->value)) update_pressure(pressure);
  
  /* Copy state to return struct */
  output_t x_out;
  x_out.h = x[0];
  x_out.s = p[0][0];
  x_out.a = x[2];

  return x_out;
}


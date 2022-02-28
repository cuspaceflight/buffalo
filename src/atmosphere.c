#include "atmosphere.h"

#include <math.h>

/* Constants from the US Standard Atmosphere 1976 */
static const float Rs = 8.31432f;
static const float g0 = 9.80665f;
static const float M = 0.0289644f;
static const float Lb[7] = {-0.0065f, 0.0f,     0.001,  0.0028f,
                            0.0f,     -0.0028f, -0.002f};
static const float Pb[7] = {101325.0f, 22632.10f, 5474.89f, 868.02f,
                            110.91f,   66.94f,    3.96f};
static const float Tb[7] = {288.15f, 216.65, 216.65, 228.65,
                            270.65,  270.65, 214.65};
static const float Hb[7] = {0.0f,     11000.0f, 20000.0f, 32000.0f,
                            47000.0f, 51000.0f, 71000.0f};

/* Functions to convert pressures to altitudes via the
 * US Standard Atmosphere
 */
float state_estimation_pressure_to_altitude(float pressure);
static float state_estimation_p2a_nonzero_lapse(float p, int b);
static float state_estimation_p2a_zero_lapse(float p, int b);

float state_estimation_pressure_to_altitude(float pressure) {
  int b;
  /* If the pressure is below the model's ground level, we can
   * extrapolate into the ground instead.
   */
  if (pressure > Pb[0]) {
    return state_estimation_p2a_nonzero_lapse(pressure, 0);
  }
  /* For each level of the US Standard Atmosphere 1976, check if the pressure
   * is inside that level, and use the appropriate conversion based on lapse
   * rate at that level.
   */
  for (b = 0; b < 6; b++) {
    if (pressure <= Pb[b] && pressure > Pb[b + 1]) {

      if (Lb[b] == 0.0f)
        return state_estimation_p2a_zero_lapse(pressure, b);

      return state_estimation_p2a_nonzero_lapse(pressure, b);
    }
  }
  /* If no levels matched, something is wrong, returning -9999f will cause
   * this pressure value to be ignored.
   */
  return -9999.0f;
}

/*
 * Convert a pressure and an atmospheric level b into an altitude.
 * Rearranges the standard equation for non-zero-lapse regions,
 * P = Pb (Tb / (Tb + Lb(h - hb)))^(M g0 / R* Lb)
 */
static float state_estimation_p2a_nonzero_lapse(float pressure, int b) {
  const float lb = Lb[b];
  const float hb = Hb[b];
  const float pb = Pb[b];
  const float tb = Tb[b];
  return hb + tb / lb * (powf(pressure / pb, -Rs * lb / g0 / M) - 1.0f);
}

/* Convert a pressure and an atmospheric level b into an altitude.
 * Reverses the standard equation for zero-lapse regions,
 * P = Pb exp( -g0 M (h-hb) / R* Tb)
 */
static float state_estimation_p2a_zero_lapse(float pressure, int b) {
  const float hb = Hb[b];
  const float pb = Pb[b];
  const float tb = Tb[b];
  return hb - Rs * tb / g0 / M * logf(pressure / pb);
}


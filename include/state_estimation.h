
#ifndef STATE_ESTIMATION_H
#define STATE_ESTIMATION_H

#include <stdint.h>

typedef struct { float h; float v; float a; } state_estimate_t;

/* Used to signal that the barometer is not trustworthy due to
 * transonic regime. Set by the mission control thread and read by
 * the barometer thread when it goes to update the state estimate.
 */
extern volatile bool state_estimation_trust_barometer;

/* Used to signal that an exciting event is expected sometime soon.
 * Specifically this means that the process noise variance in the kalman filter
 * will be higher, useful when we expect the dynamics to rapidly do something
 * interesting, e.g. motor ignition or parachute deployment.
 */
extern volatile bool state_estimation_dynamic_event_expected;

/* Update with a new pressure reading (in Pascals) and associated RMS noise */
void state_estimation_new_pressure(float pressure, float rms);

/* Update with new accelerometer readings (in m/s/s on x, y, z) and
 * associated maximum value and RMS noise */
void state_estimation_new_accels(float accels[3], float max, float rms);

/* Compute and return the latest state estimate */
state_estimate_t state_estimation_get_state(void);

/* Initialise state estimation. Must be called before
 * update or prediction steps above are called. */
void state_estimation_init(void);

#endif

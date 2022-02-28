
#ifndef STATE_ESTIMATION_H
#define STATE_ESTIMATION_H

#include <stdint.h>
#include <stdbool.h>

typedef struct { float h, v, a; } state_estimate_t;
typedef struct { float h, s, a; } output_t;
typedef struct { float value, rms; } gaussian_t;

output_t state_estimation_tick(float dt, gaussian_t* pressure,
                                         gaussian_t* accel);

#endif


#ifndef STATE_ESTIMATION_H
#define STATE_ESTIMATION_H

#include <stdint.h>
#include <stdbool.h>

typedef struct { float h, v, a; } state_estimate_t;
typedef struct { float h, s; } output_t;

output_t state_estimation_tick(float dt, float pressure, float p_rms);

#endif

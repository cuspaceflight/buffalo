
#include <stdio.h>

#include "state_estimation.h"

#define N_DATA 4

float ts[] = {1.0, 2.0, 3.0, 4.0};

float bs[] = {1.0, 2.0, 3.0, 4.0};

int main(int argc, char **argv) {
  state_estimate_t se;
  state_estimation_init();
  for (int i = 1; i < N_DATA; i++) {
    se = state_estimation_get_state(ts[i] - ts[i - 1]);
    state_estimation_new_pressure(bs[1], 0.01 * bs[1]);
    printf("%f %f %f\n", se.h, se.v, se.a);
  }

  return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                    BUFFALO                                   *
*                       FLIGHT COMPUTER SIMULATION SYSTEM                      *
*                                                                              *
*                  COPYRIGHT (C) 2021 H.E. FRANKS, B.M. ANDREW                 *
*                        CAMBRIDGE UNIVERSITY SPACEFLIGHT                      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>

#include "state_estimation.h"

#define FILE_PATH "/home/ben/projects/buffalo/flight_log/combined.bin"

enum Reading { BARO, ACCEL };

typedef struct {
  enum Reading type;
  float time;
  float value;
} reading_t;

size_t read_logfile(const char *path, reading_t *buffer) {
  FILE *fp;
  size_t size;
  fp = fopen(path, "rb");
  if (fp == NULL) {
    printf("Error opening file %s \n", path);
    return 0;
  }
  // Get size of file to allocate buffer
  fseek(fp, 0, SEEK_END);
  size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  buffer = malloc(size);
  // Read data into the buffer
  if (fread(buffer, sizeof(char), size, fp) != size) {
    printf("Error reading file %s \n", path);
    fclose(fp);
    free(buffer);
    return 0;
  }
  fclose(fp);
  return size;
}

int main(int argc, char **argv) {
  reading_t *readings;
  size_t n_readings = read_logfile(FILE_PATH, readings);

  // state_estimate_t se;
  // state_estimation_init();
  // for (int i = 1; i < 1; i++) {
  //   se = state_estimation_get_state(ts[i] - ts[i - 1]);
  //   state_estimation_new_pressure(bs[1], 0.01 * bs[1]);
  //   printf("%f %f %f\n", se.h, se.v, se.a);
  // }

  free(readings);
  return 0;
}

#ifndef FLIGHT_H
#define FLIGHT_H

#include <vector.h>
#include <atmosphere.h>

float* altitudes;
atmos_t* pressures; // baro (???)
vector_t* accelerations; // accel
vector_t* gyros; // gyro (3 component rotation about each axis?)

#endif


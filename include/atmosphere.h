#ifndef ATMOS_H
#define ATMOS_H

typedef struct atmos_datapoint { float h, p };

/* define the US Standard Atmosphere 1979 */
const atmos_datapoint US_SA_1979[10];

struct Atmosphere {
}

typedef struct Atmosphere atmos_t;

#endif


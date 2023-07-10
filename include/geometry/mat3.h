#ifndef MAT3_H
#define MAT3_H

#include "geometry/vec3.h"

struct Mat3 {
    float** data;
};

typedef struct Mat3 mat3;

mat3* mat3_create();
void mat3_free(mat3* M);

void mat3_assign_values(
    mat3* M,
    float a1, float a2, float a3,
    float b1, float b2, float b3,
    float c1, float c2, float c3
);

mat3* mat3_init(
    float a1, float a2, float a3,
    float b1, float b2, float b3,
    float c1, float c2, float c3
);

/* operators */
void  mat3_cpy(const mat3* lhs, const mat3* rhs);
void  mat3_add_inplace(const mat3* lhs, const mat3* rhs);
void  mat3_sub_inplace(const mat3* lhs, const mat3* rhs);
mat3* mat3_add(const mat3* lhs, const mat3* rhs);
mat3* mat3_sub(const mat3* lhs, const mat3* rhs);
mat3* mat3_mul(const mat3* lhs, const mat3* rhs);
vec3* mat3_mlv(const mat3* lhs, const vec3* rhs);
mat3* mat3_ml3(const mat3* A, const mat3* B, const mat3* C);
mat3* mat3_opd(const vec3* lhs, const vec3* rhs);
void  mat3_scl(const mat3* lhs, const float rhs);

/* misc matrix operators */
mat3* mat3_transpose(const mat3* M);

#endif


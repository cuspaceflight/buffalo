#ifndef MAT3_H
#define MAT3_H

struct Mat3 {
    float** data;
};

void mat3_assign_values(
    struct Mat3* M,
    float a1, float a2, float a3,
    float b1, float b2, float b3,
    float c1, float c2, float c3
);

typedef struct Mat3 mat3;

/* operators */
mat3 mat3_add(const mat3* lhs, const mat3* rhs);
mat3 mat3_sub(const mat3* lhs, const mat3* rhs);
mat3 mat3_mul(const mat3* lhs, const mat3* rhs);

/* misc matrix operators */
mat3 mat3_transpose(const mat3* M);

#endif


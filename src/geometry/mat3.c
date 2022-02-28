#include "mat3.h"

#include <stdlib.h>

inline void mat3_malloc(mat3* M)
{
    /* check for memory already allocated */
    if (M->data != NULL) return;

    M->data = malloc(3 * sizeof(float));

    for (size_t i = 0; i < 3; i++) {
        if (M->data[i] == NULL)
            M->data[i] = calloc(3, sizeof(float));
    }
}

inline void mat3_free(mat3* M)
{
    /* check for double free */
    if (M->data == NULL) return;

    for (size_t i = 0; i < 3; i++) {
        if (M->data[i] != NULL)
            free(M->data[i]);
    }

    free(M->data);
}

inline mat3 mat3_create()
{
    mat3* ret = malloc(sizeof(mat3));
    mat3_malloc(ret);

    return *ret;
}

void mat3_assign_values(
    mat3* M,
    float a1, float a2, float a3,
    float b1, float b2, float b3,
    float c1, float c2, float c3)
{
    mat3_malloc(M);

    M->data[0][0] = a1;
    M->data[0][1] = a2;
    M->data[0][2] = a3;
    M->data[1][0] = b1;
    M->data[1][1] = b2;
    M->data[1][2] = b3;
    M->data[2][0] = c1;
    M->data[2][1] = c2;
    M->data[2][2] = c3;
}

/* operators */

mat3 mat3_add(const mat3* lhs, const mat3* rhs)
{
    mat3 ret = mat3_create();

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            ret.data[i][j] = lhs->data[i][j] + rhs->data[i][j];
        }
    }

    return ret;
}

mat3 mat3_sub(const mat3* lhs, const mat3* rhs)
{
    mat3 ret = mat3_create();

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            ret.data[i][j] = lhs->data[i][j] - rhs->data[i][j];
        }
    }

    return ret;
}

mat3 mat3_mul(const mat3* lhs, const mat3* rhs)
{
    mat3 ret = mat3_create();

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            for (size_t k = 0; k < 3; k++) {
                ret.data[i][j] += lhs->data[i][k] * rhs->data[k][j];
            }
        }
    }

    return ret;
}

/* misc matrix operations */

mat3 mat3transpose(const mat3* M)
{
    mat3 ret;

    mat3_assign_values(
        &ret,
        M->data[0][0], M->data[1][0], M->data[2][0],
        M->data[0][1], M->data[1][1], M->data[2][1],
        M->data[0][2], M->data[1][2], M->data[2][2]
    );
    
    return ret;
}


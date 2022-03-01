#include "geometry/mat3.h"
#include "geometry/vec3.h"

#include <stdio.h>
#include <stdlib.h>

void mat3_malloc(mat3* M)
{
    M->data = malloc(3 * sizeof(float*));

    for (size_t i = 0; i < 3; i++) {
            M->data[i] = calloc(3, sizeof(float));
    }
}

mat3* mat3_create()
{
    mat3* ret = malloc(sizeof(mat3));
    mat3_malloc(ret);

    return ret;
}

void mat3_free(mat3* M)
{
    for (size_t i = 0; i < 3; i++)
            free(M->data[i]);

    free(M->data);
}

void mat3_assign_values(
    mat3* M,
    float a1, float a2, float a3,
    float b1, float b2, float b3,
    float c1, float c2, float c3)
{
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

mat3* mat3_init(
    float a1, float a2, float a3,
    float b1, float b2, float b3,
    float c1, float c2, float c3)
{
    mat3* ret = mat3_create();

    mat3_assign_values(
        ret,
        a1, a2, a3,
        b1, b2, b3,
        c1, c2, c3
    );

    return ret;
}

/* operators */

void  mat3_cpy(const mat3* lhs, const mat3* rhs)
{
    for (size_t i = 0; i < 3; i++) {
        for(size_t j = 0; j < 3; j++) {
            lhs->data[i][j] = rhs->data[i][j];
        }
    }
}

void mat3_add_inplace(const mat3* lhs, const mat3* rhs)
{
    for (size_t i = 0; i < 3; i++) {
        for(size_t j = 0; j < 3; j++) {
            lhs->data[i][j] += rhs->data[i][j];
        }
    }
}

void mat3_sub_inplace(const mat3* lhs, const mat3* rhs)
{
    for (size_t i = 0; i < 3; i++) {
        for(size_t j = 0; j < 3; j++) {
            lhs->data[i][j] -= rhs->data[i][j];
        }
    }
}


mat3* mat3_add(const mat3* lhs, const mat3* rhs)
{
    mat3* ret = mat3_create();

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            ret->data[i][j] = lhs->data[i][j] + rhs->data[i][j];
        }
    }

    return ret;
}

mat3* mat3_sub(const mat3* lhs, const mat3* rhs)
{
    mat3* ret = mat3_create();

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            ret->data[i][j] = lhs->data[i][j] - rhs->data[i][j];
        }
    }

    return ret;
}

mat3* mat3_mul(const mat3* lhs, const mat3* rhs)
{
    mat3* ret = mat3_create();

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            for (size_t k = 0; k < 3; k++) {
                ret->data[i][j] += lhs->data[i][k] * rhs->data[k][j];
            }
        }
    }

    return ret;
}

vec3* mat3_mlv(const mat3* lhs, const vec3* rhs)
{
    vec3* ret = vec3_create();

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            ret->data[i] += lhs->data[i][j] * rhs->data[j];
        }
    }

    return ret;
}

mat3* mat3_ml3(const mat3* A, const mat3* B, const mat3* C)
{
    mat3* BC = mat3_mul(B, C);
    mat3* ABC = mat3_mul(A, BC);
    mat3_free(BC);
    return ABC;
}

/* misc matrix operations */

mat3* mat3_transpose(const mat3* M)
{
    mat3* ret = mat3_create();

    mat3_assign_values(
        ret,
        M->data[0][0], M->data[1][0], M->data[2][0],
        M->data[0][1], M->data[1][1], M->data[2][1],
        M->data[0][2], M->data[1][2], M->data[2][2]
    );
    
    return ret;
}


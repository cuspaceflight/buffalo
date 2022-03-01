#include "geometry/vec3.h"

#include <stdio.h>
#include <stdlib.h>

void vec3_malloc(vec3* V)
{
    V->data = calloc(3, sizeof(float));
}

vec3* vec3_create()
{
    vec3* ret = malloc(sizeof(vec3));
    vec3_malloc(ret);

    return ret;
}

void vec3_free(vec3* V)
{
    free(V->data);
}

void vec3_assign_values(vec3* V, float a, float b, float c)
{
    vec3_malloc(V);

    V->data[0] = a;
    V->data[1] = b;
    V->data[2] = c;
}

vec3* vec3_init(float a, float b, float c)
{
    vec3* ret = vec3_create();

    vec3_assign_values(ret, a, b, c);

    return ret;
}

/* operators */

void  vec3_cpy(const vec3* lhs, const vec3* rhs)
{
    for (size_t i = 0; i < 3; i++)
        lhs->data[i] = rhs->data[i];
}

void  vec3_add_inplace(const vec3* lhs, const vec3* rhs)
{
    for (size_t i = 0; i < 3; i++)
        lhs->data[i] += rhs->data[i];
}

void  vec3_sub_inplace(const vec3* lhs, const vec3* rhs)
{
    for (size_t i = 0; i < 3; i++)
        lhs->data[i] -= rhs->data[i];
}

void  vec3_mul_inplace(const vec3* lhs, const float rhs)
{
    for (size_t i = 0; i < 3; i++)
        lhs->data[i] *= rhs;
}

vec3* vec3_add(const vec3* lhs, const vec3* rhs)
{
    vec3* ret = vec3_create();

    for (size_t i = 0; i < 3; i++) {
        ret->data[i] = lhs->data[i] + rhs->data[i];
    }

    return ret;
}

vec3* vec3_sub(const vec3* lhs, const vec3* rhs)
{
    vec3* ret = vec3_create();

    for (size_t i = 0; i < 3; i++) {
        ret->data[i] = lhs->data[i] - rhs->data[i];
    }

    return ret;
}

vec3* vec3_mul(const vec3* lhs, const float rhs)
{
    vec3* ret = vec3_create();

    for (size_t i = 0; i < 3; i++) {
        ret->data[i] = lhs->data[i] * rhs;
    }

    return ret;
}


#ifndef VEC3_H
#define VEC3_H

struct Vec3 {
    float* data;
};

typedef struct Vec3 vec3;

vec3* vec3_create();
void vec3_free();

void vec3_assign_values(vec3* V, float a, float b, float c);

vec3* vec3_init(float a, float b, float c);

/* operators */
void  vec3_cpy(const vec3* lhs, const vec3* rhs);
void  vec3_add_inplace(const vec3* lhs, const vec3* rhs);
void  vec3_sub_inplace(const vec3* lhs, const vec3* rhs);
void  vec3_mul_inplace(const vec3* lhs, const float rhs);
vec3* vec3_add(const vec3* lhs, const vec3* rhs);
vec3* vec3_sub(const vec3* lhs, const vec3* rhs);
vec3* vec3_mul(const vec3* lhs, const float rhs);

#endif


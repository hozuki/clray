//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_VEC3_H
#define CLRAY_VEC3_H

#include "utils.h"

typedef struct vec3_t {
    float x;
    float y;
    float z;
} vec3;

void vec3_set(vec3 *vec, float x, float y, float z);

float vec3_length(vec3 *vec);

float vec3_length_squared(vec3 *vec);

void vec3_normalize(vec3 *vec, vec3 *result);

void vec3_add(vec3 *v1, vec3 *v2, vec3 *result);

void vec3_sub(vec3 *v1, vec3 *v2, vec3 *result);

void vec3_mul_vec(vec3 *v1, vec3 *v2, vec3 *result);

void vec3_mul(vec3 *vec, float f, vec3 *result);

void vec3_div(vec3 *vec, float f, vec3 *result);

float vec3_dot(vec3 *v1, vec3 *v2);

void vec3_cross(vec3 *v1, vec3 *v2, vec3 *result);

void vec3_lerp(vec3 *v1, vec3 *v2, float amount, vec3 *result);

void vec3_reflect(vec3 *v, vec3 *n, vec3 *result);

void vec3_negate(vec3 *v, vec3 *result);

void vec3_copy(vec3 *src, vec3 *dst);

#endif //CLRAY_VEC3_H

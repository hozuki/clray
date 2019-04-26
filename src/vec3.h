//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_VEC3_H
#define CLRAY_VEC3_H

#include "opencl_compat.h"

#if !defined(__IN_OPENCL__)

typedef cl_float3 vec3;

#else

typedef float3 vec3;

#endif

float vec3_length(const vec3 vec);

float vec3_length_squared(const vec3 vec);

vec3 vec3_normalize(const vec3 v);

vec3 vec3_add(const vec3 v1, const vec3 v2);

vec3 vec3_sub(const vec3 v1, const vec3 v2);

vec3 vec3_mul_vec(const vec3 v1, const vec3 v2);

vec3 vec3_mul(const vec3 vec, float f);

vec3 vec3_div(const vec3 vec, float f);

float vec3_dot(const vec3 v1, const vec3 v2);

vec3 vec3_cross(const vec3 v1, const vec3 v2);

vec3 vec3_lerp(const vec3 v1, const vec3 v2, float amount);

vec3 vec3_reflect(const vec3 v, const vec3 n);

vec3 vec3_negate(const vec3 v);

void vec3_set(vec3 *v, float x, float y, float z);

vec3 vec3_pow(const vec3 v, float p);

#endif //CLRAY_VEC3_H

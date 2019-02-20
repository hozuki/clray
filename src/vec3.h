//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_VEC3_H
#define CLRAY_VEC3_H

#include "compat.h"

typedef struct vec3_t {
    float x;
    float y;
    float z;
    // Serves as padding
    float w;
} vec3;

EXTERN_C float vec3_length(const vec3 vec);

EXTERN_C float vec3_length_squared(const vec3 vec);

EXTERN_C vec3 vec3_normalize(const vec3 v);

EXTERN_C vec3 vec3_add(const vec3 v1, const vec3 v2);

EXTERN_C vec3 vec3_sub(const vec3 v1, const vec3 v2);

EXTERN_C vec3 vec3_mul_vec(const vec3 v1, const vec3 v2);

EXTERN_C vec3 vec3_mul(const vec3 vec, float f);

EXTERN_C vec3 vec3_div(const vec3 vec, float f);

EXTERN_C float vec3_dot(const vec3 v1, const vec3 v2);

EXTERN_C vec3 vec3_cross(const vec3 v1, const vec3 v2);

EXTERN_C vec3 vec3_lerp(const vec3 v1, const vec3 v2, float amount);

EXTERN_C vec3 vec3_reflect(const vec3 v, const vec3 n);

EXTERN_C vec3 vec3_negate(const vec3 v);

EXTERN_C void vec3_set(vec3 *v, float x, float y, float z);

#endif //CLRAY_VEC3_H

//
// Created by MIC on 2019-02-16.
//

#if !defined(__IN_OPENCL__)

#include <math.h>

#include "utils.h"

#endif

#include "vec3.h"

#define USE_EMULATED_IMPL (1)

float vec3_length(const vec3 vec) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    return sqrtf(vec3_length_squared(vec));
#else
    return length(vec);
#endif
}

float vec3_length_squared(const vec3 vec) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
#else
    return pown(length(vec), 2);
#endif
}

vec3 vec3_normalize(const vec3 vec) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    float length = vec3_length(vec);
    return vec3_div(vec, length);
#else
    return normalize(vec);
#endif
}

vec3 vec3_add(const vec3 v1, const vec3 v2) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    vec3 result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;

    return result;
#else
    return v1 + v2;
#endif
}

vec3 vec3_sub(const vec3 v1, const vec3 v2) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    vec3 result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;

    return result;
#else
    return v1 - v2;
#endif
}

vec3 vec3_mul_vec(const vec3 v1, const vec3 v2) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    vec3 result;

    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    result.z = v1.z * v2.z;

    return result;
#else
    return v1 * v2;
#endif
}

vec3 vec3_mul(const vec3 vec, float f) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    vec3 result;

    result.x = vec.x * f;
    result.y = vec.y * f;
    result.z = vec.z * f;

    return result;
#else
    return vec * f;
#endif
}

vec3 vec3_div(const vec3 vec, float f) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    vec3 result;

    result.x = vec.x / f;
    result.y = vec.y / f;
    result.z = vec.z / f;

    return result;
#else
    return vec / f;
#endif
}

float vec3_dot(const vec3 v1, const vec3 v2) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
#else
    return dot(v1, v2);
#endif
}

vec3 vec3_cross(const vec3 v1, const vec3 v2) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    vec3 result;

    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;

    return result;
#else
    return cross(v1, v2);
#endif
}

vec3 vec3_lerp(const vec3 v1, const vec3 v2, float amount) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    vec3 result;

    amount = CLAMP(amount, 0, 1);

    float mt = 1 - amount;

    result.x = mt * v1.x + amount * v2.x;
    result.y = mt * v1.y + amount * v2.y;
    result.z = mt * v1.z + amount * v2.z;

    return result;
#else
    return v1 * (1 - amount) + v2 * amount;
#endif
}

vec3 vec3_reflect(const vec3 v, const vec3 n) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    float p = 2 * vec3_dot(v, n);
    return vec3_sub(n, vec3_mul(n, p));
#else
    return v - 2.0f * dot(v, n) * n;
#endif
}

vec3 vec3_negate(const vec3 v) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    vec3 result;

    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;

    return result;
#else
    return -v;
#endif
}

void vec3_set(vec3 *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
}

vec3 vec3_pow(const vec3 v, float p) {
#if !defined(__IN_OPENCL__) || USE_EMULATED_IMPL
    vec3 result;

    result.x = powf(v.x, p);
    result.y = powf(v.y, p);
    result.z = powf(v.z, p);

    return result;
#else
    return pow(v, p);
#endif
}

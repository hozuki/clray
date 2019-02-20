//
// Created by MIC on 2019-02-16.
//

#include <math.h>

#include "utils.h"
#include "vec3.h"

EXTERN_C float vec3_length(const vec3 vec) {
    return sqrtf(vec3_length_squared(vec));
}

EXTERN_C float vec3_length_squared(const vec3 vec) {
    return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

EXTERN_C vec3 vec3_normalize(const vec3 vec) {
    float length = vec3_length(vec);
    return vec3_div(vec, length);
}

EXTERN_C vec3 vec3_add(const vec3 v1, const vec3 v2) {
    vec3 result;

    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    result.w = 0;

    return result;
}

EXTERN_C vec3 vec3_sub(const vec3 v1, const vec3 v2) {
    vec3 result;

    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    result.w = 0;

    return result;
}

EXTERN_C vec3 vec3_mul_vec(const vec3 v1, const vec3 v2) {
    vec3 result;

    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    result.z = v1.z * v2.z;
    result.w = 0;

    return result;
}

EXTERN_C vec3 vec3_mul(const vec3 vec, float f) {
    vec3 result;

    result.x = vec.x * f;
    result.y = vec.y * f;
    result.z = vec.z * f;
    result.w = 0;

    return result;
}

EXTERN_C vec3 vec3_div(const vec3 vec, float f) {
    vec3 result;

    result.x = vec.x / f;
    result.y = vec.y / f;
    result.z = vec.z / f;
    result.w = 0;

    return result;
}

EXTERN_C float vec3_dot(const vec3 v1, const vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

EXTERN_C vec3 vec3_cross(const vec3 v1, const vec3 v2) {
    vec3 result;

    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    result.w = 0;

    return result;
}

EXTERN_C vec3 vec3_lerp(const vec3 v1, const vec3 v2, float amount) {
    vec3 result;

    amount = CLAMP(amount, 0, 1);

    float mt = 1 - amount;

    result.x = mt * v1.x + amount * v2.x;
    result.y = mt * v1.y + amount * v2.y;
    result.z = mt * v1.z + amount * v2.z;
    result.w = 0;

    return result;
}

EXTERN_C vec3 vec3_reflect(const vec3 v, const vec3 n) {
    float p = 2 * vec3_dot(v, n);
    return vec3_sub(n, vec3_mul(n, p));
}

EXTERN_C vec3 vec3_negate(const vec3 v) {
    vec3 result;

    result.x = -v.x;
    result.y = -v.y;
    result.z = -v.z;
    result.w = 0;

    return result;
}

EXTERN_C void vec3_set(vec3 *v, float x, float y, float z) {
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = 0;
}

//
// Created by MIC on 2019-02-16.
//

#include <math.h>

#include "utils.h"
#include "vec3.h"

float vec3_length(vec3 *vec) {
    return sqrtf(vec3_length_squared(vec));
}

float vec3_length_squared(vec3 *vec) {
    return vec->x * vec->x + vec->y * vec->y + vec->z * vec->z;
}

void vec3_normalize(vec3 *vec, vec3 *result) {
    float length = vec3_length(vec);
    vec3_div(vec, length, result);
}

void vec3_add(vec3 *v1, vec3 *v2, vec3 *result) {
    result->x = v1->x + v2->x;
    result->y = v1->y + v2->y;
    result->z = v1->z + v2->z;
}

void vec3_sub(vec3 *v1, vec3 *v2, vec3 *result) {
    result->x = v1->x - v2->x;
    result->y = v1->y - v2->y;
    result->z = v1->z - v2->z;
}

void vec3_mul_vec(vec3 *v1, vec3 *v2, vec3 *result) {
    result->x = v1->x * v2->x;
    result->y = v1->y * v2->y;
    result->z = v1->z * v2->z;
}

void vec3_mul(vec3 *vec, float f, vec3 *result) {
    result->x = vec->x * f;
    result->y = vec->y * f;
    result->z = vec->z * f;
}

void vec3_div(vec3 *vec, float f, vec3 *result) {
    result->x = vec->x / f;
    result->y = vec->y / f;
    result->z = vec->z / f;
}

float vec3_dot(vec3 *v1, vec3 *v2) {
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

void vec3_cross(vec3 *v1, vec3 *v2, vec3 *result) {
    vec3 v1a, v2a;

    vec3_copy(v1, &v1a);
    vec3_copy(v2, &v2a);

    result->x = v1a.y * v2a.z - v1a.z * v2a.y;
    result->y = v1a.z * v2a.x - v1a.x * v2a.z;
    result->z = v1a.x * v2a.y - v1a.y * v2a.x;
}

void vec3_set(vec3 *vec, float x, float y, float z) {
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

void vec3_lerp(vec3 *v1, vec3 *v2, float amount, vec3 *result) {
    amount = CLAMP(amount, 0, 1);

    float mt = 1 - amount;

    result->x = mt * v1->x + amount * v2->x;
    result->y = mt * v1->y + amount * v2->y;
    result->z = mt * v1->z + amount * v2->z;
}

void vec3_reflect(vec3 *v, vec3 *n, vec3 *result) {
    float p = vec3_dot(v, n);
    p = 2 * p;

    vec3 nn;
    vec3_mul(n, p, &nn);

    vec3_sub(n, &nn, result);
}

void vec3_negate(vec3 *v, vec3 *result) {
    result->x = -v->x;
    result->y = -v->y;
    result->z = -v->z;
}

void vec3_copy(vec3 *src, vec3 *dst) {
    *dst = *src;
}

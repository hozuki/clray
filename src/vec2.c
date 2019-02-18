//
// Created by MIC on 2019-02-16.
//

#include "vec2.h"

float vec2_length_squared(vec2 *v) {
    return v->x * v->x + v->y * v->y;
}

void vec2_mul(vec2 *vec, float f, vec2 *result) {
    result->x = vec->x * f;
    result->y = vec->y * f;
}

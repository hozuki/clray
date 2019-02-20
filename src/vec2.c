//
// Created by MIC on 2019-02-16.
//

#include "vec2.h"

#if !defined(__IN_OPENCL__)

float vec2_length_squared(const vec2 v) {
    return v.x * v.x + v.y * v.y;
}

vec2 vec2_mul(const vec2 v, float f) {
    vec2 result;

    result.x = v.x * f;
    result.y = v.y * f;

    return result;
}

#endif

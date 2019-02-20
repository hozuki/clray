//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_VEC2_H
#define CLRAY_VEC2_H

#include "compat.h"

typedef struct vec2_t {
    float x;
    float y;
} vec2;

EXTERN_C float vec2_length_squared(const vec2 v);

EXTERN_C vec2 vec2_mul(const vec2 v, float f);

#endif //CLRAY_VEC2_H

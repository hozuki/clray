//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_VEC2_H
#define CLRAY_VEC2_H

typedef struct vec2_t {
    float x;
    float y;
} vec2;

float vec2_length_squared(vec2 *v);

void vec2_mul(vec2 *vec, float f, vec2 *result);

#endif //CLRAY_VEC2_H

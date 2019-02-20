//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_VEC2_H
#define CLRAY_VEC2_H

#if !defined(__IN_OPENCL__)

typedef struct vec2_t {
    float x;
    float y;
} vec2;

float vec2_length_squared(const vec2 v);

vec2 vec2_mul(const vec2 v, float f);

#else

typedef float2 vec2;

//#define vec2_length_squared(v) (pown(length((v)), 2))
#define vec2_length_squared(v) (pow(length((v)), 2))

#define vec2_mul(v, f) ((v) * (f))

#endif

#endif //CLRAY_VEC2_H

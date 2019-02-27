//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_UTILS_H
#define CLRAY_UTILS_H

#if !defined(__IN_OPENCL__)

#include <stdint.h>

#endif

#include "opencl_compat.h"

#define CLAMP(v, min, max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))

#include "vec3.h"
#include "vec2.h"

typedef struct frand_state_t {
    cl_ulong x;
} frand_state_t;

vec3 random_in_unit_sphere(frand_state_t *frand_state);

vec2 random_in_unit_disk(frand_state_t *frand_state);

vec3 correct_gamma(vec3 color, float gamma);

float frand(frand_state_t *state);

void frand_init(frand_state_t *state, uint64_t seed);

#if !defined(__IN_OPENCL__)

uint64_t time_ms();

uint64_t get_random_seed();

#endif

#if defined(_MSC_VER)

double drand48();

#elif defined(__IN_OPENCL__)

float drand48();

#endif

#endif //CLRAY_UTILS_H

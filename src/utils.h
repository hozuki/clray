//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_UTILS_H
#define CLRAY_UTILS_H

#include <stdint.h>

#define CLAMP(v, min, max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))

#include "compat.h"
#include "rt_options.h"
#include "vec3.h"
#include "vec2.h"

typedef struct frand_state_t {
    uint64_t x;
} frand_state_t;

EXTERN_C vec3 random_in_unit_sphere(frand_state_t *frand_state);

EXTERN_C vec2 random_in_unit_disk(frand_state_t *frand_state);

EXTERN_C vec3 correct_gamma(vec3 color, float gamma);

EXTERN_C float frand(frand_state_t *state);

EXTERN_C void frand_init(frand_state_t *state, uint64_t seed);

EXTERN_C uint64_t time_ms();

EXTERN_C uint64_t get_random_seed();

#endif //CLRAY_UTILS_H

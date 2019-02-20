//
// Created by MIC on 2019-02-16.
//

#include <stdlib.h>
#include <math.h>
#include <time.h>

#if defined(_WIN32) || defined(WIN32)

#include <Windows.h>

#endif

#include "vec3.h"
#include "vec2.h"
#include "utils.h"

EXTERN_C void frand_init(frand_state_t *state, uint64_t seed) {
#if defined(_MSC_VER) || defined(__IN_OPENCL__)
    state->x = seed;
#endif
}

EXTERN_C vec3 random_in_unit_sphere(frand_state_t *frand_state) {
    vec3 result;
    vec3_set(&result, 0, 0, 0);

    do {
        float x = frand(frand_state);
        float y = frand(frand_state);
        float z = frand(frand_state);

        x = 2 * x - 1;
        y = 2 * y - 1;
        z = 2 * z - 1;

        result.x = x;
        result.y = y;
        result.z = z;
    } while (vec3_length_squared(result) >= 1.0f);

    return result;
}

EXTERN_C vec2 random_in_unit_disk(frand_state_t *frand_state) {
    vec2 result;

    do {
        float x = frand(frand_state);
        float y = frand(frand_state);

        x = 2 * x - 1;
        y = 2 * y - 1;

        result.x = x;
        result.y = y;
    } while (vec2_length_squared(result) >= 1.0f);

    return result;
}

EXTERN_C vec3 correct_gamma(vec3 color, float gamma) {
    float gammaInv = 1 / gamma;

    vec3 result;

    result.x = powf(CLAMP(color.x, 0, 1), gammaInv);
    result.y = powf(CLAMP(color.y, 0, 1), gammaInv);
    result.z = powf(CLAMP(color.z, 0, 1), gammaInv);
    result.w = 0;

    return result;
}

#if defined(__IN_OPENCL__)
#define DEFINE_ULL(n) (n##UL)
#else
#define DEFINE_ULL(n) (n##ULL)
#endif

EXTERN_C float frand(frand_state_t *state) {
#if defined(_MSC_VER) || defined(__IN_OPENCL__)
    uint64_t a = DEFINE_ULL(0x5deece66d);
    uint64_t c = DEFINE_ULL(0xb);
    uint64_t m = DEFINE_ULL(1) << DEFINE_ULL(49);

    state->x = (state->x * a + c) % m;

    return (float)state->x / (float)((DEFINE_ULL(1) << DEFINE_ULL(49)) + 1);
#else
    return (float)drand48();
#endif
}

EXTERN_C uint64_t time_ms() {
#if defined(_WIN32) || defined(WIN32)
    return GetTickCount64();
#else
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);

    time_t s = spec.tv_sec;
    uint64_t ms = (uint64_t)round(spec.tv_nsec / 1.0e6);

    if (ms > 999) {
        s += 1;
        ms = 0;
    }

    return (uint64_t)s * 1000 + ms;
#endif
}

EXTERN_C uint64_t get_random_seed() {
#ifdef _MSC_VER
    struct {
        union {
            time_t t;
            uint64_t ull;
        };
    } v;
    v.ull = 0;
    v.t = time(NULL);
    return v.ull;
#else
    return 0;
#endif
}

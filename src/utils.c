//
// Created by MIC on 2019-02-16.
//

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "vec3.h"
#include "vec2.h"
#include "utils.h"

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

#ifdef _MSC_VER

static uint64_t _xlast;

double drand48() {
    uint64_t a = 0x5deece66dULL;
    uint64_t c = 0xbULL;
    uint64_t m = 1ULL << 49ULL;

    _xlast = (_xlast * a + c) % m;

    return (double)_xlast / (double)((1ULL << 49ULL) + 1);
}

#endif

void sdrand48() {
#ifdef _MSC_VER
    struct {
        union {
            time_t t;
            uint64_t ull;
        };
    } v;
    v.ull = 0;
    v.t = time(NULL);
    _xlast = v.ull;
#endif
}

void random_in_unit_sphere(struct vec3_t *result) {
    do {
        float x = frand();
        float y = frand();
        float z = frand();

        x = 2 * x - 1;
        y = 2 * y - 1;
        z = 2 * z - 1;

        result->x = x;
        result->y = y;
        result->z = z;
    } while (vec3_length_squared(result) >= 1.0f);
}

void random_in_unit_disk(vec2 *result) {
    do {
        float x = frand();
        float y = frand();

        x = 2 * x - 1;
        y = 2 * y - 1;

        result->x = x;
        result->y = y;
    } while (vec2_length_squared(result) >= 1.0f);
}

void correct_gamma(vec3 *color, float gamma, vec3 *result) {
    float gammaInv = 1 / gamma;

    result->x = powf(CLAMP(color->x, 0, 1), gammaInv);
    result->y = powf(CLAMP(color->y, 0, 1), gammaInv);
    result->z = powf(CLAMP(color->z, 0, 1), gammaInv);

//    result->x = powf(color->x, gammaInv);
//    result->y = powf(color->y, gammaInv);
//    result->z = powf(color->z, gammaInv);
}

float frand() {
    return (float)drand48();
}

uint64_t time_ms() {
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

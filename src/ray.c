//
// Created by MIC on 2019-02-16.
//

#include "ray.h"

#if !defined(__IN_OPENCL__)

#if defined(_WIN32) || defined(WIN32)

#include <Windows.h>

#endif

// https://stackoverflow.com/a/8007608
#if defined(__GNUC__)
#define atomic_increment(ptr) __sync_fetch_and_add((ptr), 1)
#elif defined(_WIN32) || defined(WIN32)
#define atomic_increment(ptr) InterlockedIncrement64((ptr))
#else
#error Cannot find an atomic increment implementation.
#endif

#endif

void ray_set(ray_t *ray, const vec3 *origin, const vec3 *direction) {
    ray->origin = *origin;
    ray->direction = vec3_normalize(*direction);
}

vec3 ray_point_at(const ray_t *ray, float t) {
    return vec3_add(vec3_mul(ray->direction, t), ray->origin);
}

#if !defined(__IN_OPENCL__)

static volatile int64_t _ray_count = 0;

int64_t ray_get_sample_count() {
    return _ray_count;
}

void ray_reset_sample_count() {
    _ray_count = 0;
}

void ray_increment_sample_count() {
    atomic_increment(&_ray_count);
}

#endif

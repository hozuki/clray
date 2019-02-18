//
// Created by MIC on 2019-02-16.
//

#include "ray.h"

#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

// https://stackoverflow.com/a/8007608
#if defined(__GNUC__)
#define atomic_increment(ptr) __sync_fetch_and_add((ptr), 1)
#elif defined(_WIN32) || defined(WIN32)
#define atomic_increment(ptr) InterlockedIncrement((ptr))
#else
#error Cannot find an atomic increment implementation.
#endif

void ray_set(ray_t *ray, vec3 *origin, vec3 *direction) {
    ray->origin = *origin;

    vec3 dir;
    vec3_normalize(direction, &dir);

    ray->direction = dir;
}

void ray_point_at(ray_t *ray, float t, vec3 *result) {
    vec3 r1;

    vec3_mul(&ray->direction, t, &r1);
    vec3_add(&ray->origin, &r1, result);
}

static uint64_t _ray_count = 0;

uint64_t ray_get_sample_count() {
    return _ray_count;
}

void ray_reset_sample_count() {
    _ray_count = 0;
}

void ray_increment_sample_count() {
    atomic_increment(&_ray_count);
}

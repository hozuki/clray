//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_RAY_H
#define CLRAY_RAY_H

#if !defined(__IN_OPENCL__)

#include <stdint.h>

#endif

#include "vec3.h"
#include "camera.h"
#include "pbr_material.h"

typedef struct ray_t {
    vec3 origin;
    vec3 direction;
} ray_t;

#include "opencl_compat.h"

void ray_set(ray_t *ray, const vec3 *origin, const vec3 *direction);

vec3 ray_point_at(const ray_t *ray, float t);

#if !defined(__IN_OPENCL__)

int64_t ray_get_sample_count();

void ray_reset_sample_count();

void ray_increment_sample_count();

#endif

#endif //CLRAY_RAY_H

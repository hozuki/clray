//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_RAY_H
#define CLRAY_RAY_H

#include <stdint.h>

#include "vec3.h"
#include "camera.h"
#include "pbr_material.h"

typedef struct ray_t {
    vec3 origin;
    vec3 direction;
} ray_t;

void ray_set(ray_t *ray, vec3 *origin, vec3 *direction);

void ray_point_at(ray_t *ray, float t, vec3 *result);

uint64_t ray_get_sample_count();

void ray_reset_sample_count();

void ray_increment_sample_count();

#endif //CLRAY_RAY_H

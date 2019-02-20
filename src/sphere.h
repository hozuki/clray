//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_SPHERE_H
#define CLRAY_SPHERE_H

#if !defined(__IN_OPENCL__)

#include <stdbool.h>
#include <stdlib.h>

#endif

#include "vec3.h"
#include "pbr_material.h"
#include "opencl_compat.h"

typedef struct sphere_t {
    vec3 center;
    float radius;
    pbr_material_t material;
} sphere_t;

struct hit_record_t;

struct ray_t;

bool ray_hit_sphere(const struct ray_t *ray, __global const sphere_t *sphere, float minDist, float maxDist, struct hit_record_t *rec);

bool ray_hit_spheres(const struct ray_t *ray, __global const sphere_t *spheres, size_t n, float minDist, float maxDist, struct hit_record_t *rec);

#endif //CLRAY_SPHERE_H

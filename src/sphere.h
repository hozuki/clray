//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_SPHERE_H
#define CLRAY_SPHERE_H

#include <stdbool.h>
#include <stdlib.h>

#include "vec3.h"
#include "ray.h"
#include "pbr_material.h"

typedef struct sphere_t {
    vec3 center;
    float radius;
    pbr_material_t material;
} sphere_t;

struct hit_record_t;

bool ray_hit_sphere(ray_t *ray, sphere_t *sphere, float minDist, float maxDist, struct hit_record_t *rec);

bool ray_hit_spheres(ray_t *ray, sphere_t *spheres, size_t n, float minDist, float maxDist, struct hit_record_t *rec);

#endif //CLRAY_SPHERE_H

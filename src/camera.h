//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_CAMERA_H
#define CLRAY_CAMERA_H

#if !defined(__IN_OPENCL__)

#include <stdbool.h>

#endif

#include "vec3.h"
#include "opencl_compat.h"

struct ray_t;

struct frand_state_t;

typedef struct camera_t {
    vec3 origin;
    vec3 center;
    vec3 horizontal;
    vec3 vertical;

    vec3 _front;
    vec3 _up;
    vec3 _right;
    float _lens_radius;
} camera_t;

void camera_set(camera_t *camera, vec3 *eye, vec3 *lookAt, vec3 *up, float fovDeg, float aspect, float aperture, float focalLength);

void camera_get_ray(__global const camera_t *camera, float u, float v, struct frand_state_t *frand_state, struct ray_t *ray);

#endif //CLRAY_CAMERA_H

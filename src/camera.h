//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_CAMERA_H
#define CLRAY_CAMERA_H

#include <stdbool.h>

#include "vec3.h"

struct ray_t;

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

void camera_get_ray(camera_t *camera, float u, float v, struct ray_t *ray);

#endif //CLRAY_CAMERA_H

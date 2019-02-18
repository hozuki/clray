//
// Created by MIC on 2019-02-16.
//

#define _USE_MATH_DEFINES

#include <math.h>

#include "camera.h"
#include "ray.h"
#include "vec3.h"
#include "vec2.h"
#include "utils.h"

void camera_set(camera_t *camera, vec3 *eye, vec3 *lookAt, vec3 *up, float fovDeg, float aspect, float aperture, float focalLength) {
    float theta = (float)(fovDeg * M_PI / 180);
    float halfHeight = tanf(theta / 2);
    float halfWidth = halfHeight * aspect;

    vec3_copy(eye, &camera->origin);

    vec3 u, v, w;
    vec3 tmp;

    vec3_sub(eye, lookAt, &tmp);
    vec3_normalize(&tmp, &w);
    vec3_cross(up, &w, &tmp);
    vec3_normalize(&tmp, &u);
    vec3_cross(&w, &u, &v);

    vec3_copy(&u, &camera->_right);
    vec3_copy(&v, &camera->_up);
    vec3_negate(&w, &camera->_front);

    vec3_mul(&w, focalLength, &tmp);
    vec3_sub(&camera->origin, &tmp, &camera->center);

    vec3_mul(&u, 2 * halfWidth * focalLength, &camera->horizontal);
    vec3_mul(&v, 2 * halfHeight * focalLength, &camera->vertical);

    camera->_lens_radius = aperture / 2;
}

void camera_get_ray(camera_t *camera, float u, float v, ray_t *ray) {
    u = u - 0.5f;
    v = v - 0.5f;

    vec2 rd;
    random_in_unit_disk(&rd);
    vec2_mul(&rd, camera->_lens_radius, &rd);

    vec3 d1, d2;

    vec3 offset;
    vec3_mul(&camera->_right, rd.x, &d1);
    vec3_mul(&camera->_up, rd.y, &d2);
    vec3_add(&d1, &d2, &offset);

    vec3_mul(&camera->horizontal, u, &d1);
    vec3_mul(&camera->vertical, v, &d2);

    vec3 origin;
    vec3_add(&camera->origin, &offset, &origin);

    vec3 direction;
    vec3_add(&camera->center, &d1, &direction);
    vec3_add(&direction, &d2, &direction);
    vec3_sub(&direction, &camera->origin, &direction);
    vec3_sub(&direction, &offset, &direction);

    ray_set(ray, &origin, &direction);
}

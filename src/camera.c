//
// Created by MIC on 2019-02-16.
//

#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include <stdio.h>

#include "camera.h"
#include "ray.h"
#include "vec3.h"
#include "vec2.h"
#include "utils.h"

EXTERN_C void camera_set(camera_t *camera, vec3 *eye, vec3 *lookAt, vec3 *up, float fovDeg, float aspect, float aperture, float focalLength) {
    float theta = (float)(fovDeg * M_PI / 180);
    float halfHeight = tanf(theta / 2);
    float halfWidth = halfHeight * aspect;

    camera->origin = *eye;

    vec3 u, v, w;

    w = vec3_normalize(vec3_sub(*eye, *lookAt));
    u = vec3_normalize(vec3_cross(*up, w));
    v = vec3_cross(w, u);

    camera->_right = u;
    camera->_up = v;
    camera->_front = vec3_negate(w);

    camera->center = vec3_sub(camera->origin, vec3_mul(w, focalLength));

    camera->horizontal = vec3_mul(u, 2 * halfWidth * focalLength);
    camera->vertical = vec3_mul(v, 2 * halfHeight * focalLength);

    camera->_lens_radius = aperture / 2;
}

EXTERN_C void camera_get_ray(const camera_t *camera, float u, float v, frand_state_t *frand_state, ray_t *ray) {
    u = u - 0.5f;
    v = v - 0.5f;

    vec2 rd = random_in_unit_disk(frand_state);
    rd = vec2_mul(rd, camera->_lens_radius);

    vec3 offset = vec3_add(vec3_mul(camera->_right, rd.x), vec3_mul(camera->_up, rd.y));

    vec3 d1 = vec3_mul(camera->horizontal, u);
    vec3 d2 = vec3_mul(camera->vertical, v);

    vec3 origin = vec3_add(camera->origin, offset);

    vec3 direction = vec3_sub(vec3_sub(vec3_add(vec3_add(camera->center, d1), d2), camera->origin), offset);

    ray_set(ray, &origin, &direction);
}

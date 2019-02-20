//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_RAYTRACER_H
#define CLRAY_RAYTRACER_H

#if !defined(__IN_OPENCL__)

#include <stdlib.h>

#endif

#include "vec3.h"
#include "opencl_compat.h"

struct ray_t;
struct scene_t;
struct img_t;
struct frand_state_t;

#if !defined(__IN_OPENCL__)

void compute_color_recursive(const struct ray_t *ray, const struct scene_t *scene, struct frand_state_t *frand_state, vec3 *color);

#endif

void compute_color_iterative(const struct ray_t *ray, __global const struct scene_t *scene, struct frand_state_t *frand_state, vec3 *color);

/**
 * @brief Main render procedure.
 * @param camera The camera to use.
 * @param scene The scene to render.
 * @param width Image width in pixels.
 * @param height Image height in pixels.
 * @param samples Sample count per pixel.
 * @param img The image to output to.
 */
void ray_tracer_render(__global const struct camera_t *camera, __global const struct scene_t *scene, int32_t samples, struct frand_state_t *frand_state, struct img_t *img);

void ray_tracer_enable_logging(bool enabled);

#endif //CLRAY_RAYTRACER_H

//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_RAYTRACER_H
#define CLRAY_RAYTRACER_H

#include <stdlib.h>

struct ray_t;

struct vec3_t;

struct scene_t;

struct img_t;

void compute_color_recursive(struct ray_t *ray, struct scene_t *scene, struct vec3_t *color);

void compute_color_iterative(struct ray_t *ray, struct scene_t *scene, struct vec3_t *color);

/**
 * @brief Main render procedure.
 * @param camera The camera to use.
 * @param scene The scene to render.
 * @param width Image width in pixels.
 * @param height Image height in pixels.
 * @param samples Sample count per pixel.
 * @param img The image to output to.
 */
void ray_tracer_render(struct camera_t *camera, struct scene_t *scene, int32_t samples, struct img_t *img);

void ray_tracer_enable_logging(bool enabled);

#endif //CLRAY_RAYTRACER_H

//
// Created by MIC on 2019-02-16.
//

#if !defined(__IN_OPENCL__)

#include <stdarg.h>
#include <float.h>
#include <stdio.h>
#include <stdbool.h>

#endif

#include "vec3.h"
#include "ray.h"
#include "hit_record.h"
#include "sphere.h"
#include "utils.h"
#include "scene.h"
#include "img.h"
#include "raytracer.h"

#if defined(__IN_OPENCL__)

#ifndef FLT_MAX
#define FLT_MAX MAXFLOAT
#endif

#endif

static void compute_skybox_color(const ray_t *ray, vec3 *color) {
    float t = 0.5f * (ray->direction.y + 1.0f);

    vec3 c1, c2;
    vec3_set(&c1, 1.0f, 1.0f, 1.0f);
    vec3_set(&c2, 0.5f, 0.7f, 1.0f);

    *color = vec3_lerp(c1, c2, t);
}

#define MAX_DEPTH (50)

#if !defined(__IN_OPENCL__)

static void compute_color_recursive_kernel(const ray_t *ray, const scene_t *scene, frand_state_t *frand_state, vec3 *color, cl_uint depth) {
    hit_record_t rec;

    if (ray_hit_spheres(ray, scene->objects, scene->n, 0.001f, FLT_MAX, &rec)) {
        ray_t scattered;
        vec3 attenuation;

        if (depth < MAX_DEPTH && material_scatter(&rec.material, ray, &rec, frand_state, &attenuation, &scattered)) {
            vec3 c;
            compute_color_recursive_kernel(&scattered, scene, frand_state, &c, depth + 1);

            *color = vec3_mul_vec(attenuation, c);
        } else {
            vec3_set(color, 0, 0, 0);
        }
    } else {
        compute_skybox_color(ray, color);
    }
}

void compute_color_recursive(const ray_t *ray, const scene_t *scene, frand_state_t *frand_state, vec3 *color) {
    compute_color_recursive_kernel(ray, scene, frand_state, color, 0);
}

#endif

void compute_color_iterative(const ray_t *ray, __global const scene_t *scene, frand_state_t *frand_state, vec3 *color) {
    vec3 finalColor;
    vec3_set(&finalColor, 1, 1, 1);

    cl_uint depth = 0;
    ray_t scattered;
    ray_t *inputRay = (ray_t *)ray;

    __global const sphere_t *spheres = scene->objects;
    const cl_uint n = scene->n;

    while (depth < MAX_DEPTH) {
        hit_record_t rec;

        bool b = ray_hit_spheres(inputRay, spheres, n, 0.001f, FLT_MAX, &rec);

        if (!b) {
            break;
        }

        vec3 attenuation;

        b = material_scatter(&rec.material, inputRay, &rec, frand_state, &attenuation, &scattered);

        if (b) {
            finalColor = vec3_mul_vec(finalColor, attenuation);
            inputRay = &scattered;
        } else {
            vec3_set(&finalColor, 0, 0, 0);
            break;
        }

        depth += 1;
    }

    vec3 skybox;
    compute_skybox_color(inputRay, &skybox);

    *color = vec3_mul_vec(finalColor, skybox);
}

#if !defined(__IN_OPENCL__)

static bool _log_ray_trace = false;

void ray_tracer_enable_logging(bool enabled) {
    _log_ray_trace = enabled;
}

bool ray_tracer_logging_enabled() {
    return _log_ray_trace;
}

void ray_tracer_log(const char *format, ...) {
    if (!ray_tracer_logging_enabled()) {
        return;
    }

    va_list args;

        va_start(args, format);
    vfprintf(stdout, format, args);
        va_end(args);
}

#endif

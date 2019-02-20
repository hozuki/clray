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

static void compute_color_recursive_kernel(const ray_t *ray, const scene_t *scene, frand_state_t *frand_state, vec3 *color, size_t depth) {
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

    size_t depth = 0;
    ray_t scattered;
    ray_t *inputRay = (ray_t *)ray;

    __global const sphere_t *spheres = scene->objects;
    const size_t n = scene->n;

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

static void ray_tracer_log(const char *format, ...) {
    va_list args;

        va_start(args, format);
    vfprintf(stdout, format, args);
        va_end(args);
}

#endif

void ray_tracer_render(__global const camera_t *camera, __global const scene_t *scene, int32_t samples, frand_state_t *frand_state, img_t *img) {
#if !defined(__IN_OPENCL__)
    if (_log_ray_trace) {
        ray_tracer_log("Processing start.\n");
    }
#endif

    int32_t width, height;

    img_get_size(img, &width, &height);

#if !defined(__IN_OPENCL__)
    uint64_t timeStart = time_ms();
#endif

    for (int32_t j = 0; j < height; j += 1) {
        vec3 color;
        vec3_set(&color, 0, 0, 0);

        for (int32_t i = 0; i < width; i += 1) {
            vec3 c;
            vec3_set(&c, 0, 0, 0);

            for (int32_t s = 0; s < samples; s += 1) {
                float u = (i + frand(frand_state)) / (float)width;
                float v = (j + frand(frand_state)) / (float)height;
                ray_t ray;

                camera_get_ray(camera, u, v, frand_state, &ray);
                compute_color_iterative(&ray, scene, frand_state, &c);

                color = vec3_add(color, c);
            }

            color = vec3_div(color, (float)samples);

            color = correct_gamma(color, 2.0f);

            img_set_pixel(img, i, j, &color);
        }

#if !defined(__IN_OPENCL__)
        if (_log_ray_trace) {
            uint64_t t = time_ms();
            float elapsed = (t - timeStart) / 1000.0f;
            float remaining = elapsed * (height - j - 1) / (j + 1);
            ray_tracer_log("Processed %d / %d rows; elapsed: %.3f sec; ETA: %.3f sec (%.2f min)\n",
                           j + 1, height, elapsed, remaining, remaining / 60);
        }
#endif
    }

#if !defined(__IN_OPENCL__)
    uint64_t timeEnd = time_ms();

    if (_log_ray_trace) {
        ray_tracer_log("Ray tracing complete.\n");
    }

    int64_t sampleCount = ray_get_sample_count();
    float timeInSeconds = (timeEnd - timeStart) / 1000.0f;

    if (_log_ray_trace) {
        // https://computergraphics.stackexchange.com/a/2380
        ray_tracer_log("Total time: %.3f s (%.2f min), %.3lf sample/s\n",
                       timeInSeconds, timeInSeconds / 60, (double)sampleCount / timeInSeconds);
    }
#endif
}

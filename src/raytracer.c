//
// Created by MIC on 2019-02-16.
//

#include <float.h>
#include <stdio.h>
#include <stdbool.h>

#include "vec3.h"
#include "ray.h"
#include "hit_record.h"
#include "sphere.h"
#include "utils.h"
#include "scene.h"
#include "img.h"
#include "raytracer.h"

void compute_skybox_color(ray_t *ray, vec3 *color) {
    float t = 0.5f * (ray->direction.y + 1.0f);

    vec3 c1, c2;
    vec3_set(&c1, 1.0f, 1.0f, 1.0f);
    vec3_set(&c2, 0.5f, 0.7f, 1.0f);

    vec3_lerp(&c1, &c2, t, color);
}

size_t MAX_DEPTH = 50;

void compute_color_recursive_kernel(ray_t *ray, scene_t *scene, vec3 *color, size_t depth) {
    hit_record_t rec;

    if (ray_hit_spheres(ray, scene->objects, scene->n, 0.001f, FLT_MAX, &rec)) {
        ray_t scattered;
        vec3 attenuation;

        if (depth < MAX_DEPTH && material_scatter(&rec.material, ray, &rec, &attenuation, &scattered)) {
            vec3 c;
            compute_color_recursive_kernel(&scattered, scene, &c, depth + 1);

            vec3_mul_vec(&attenuation, &c, color);
        } else {
            vec3_set(color, 0, 0, 0);
        }
    } else {
        compute_skybox_color(ray, color);
    }
}

void compute_color_recursive(ray_t *ray, scene_t *scene, vec3 *color) {
    compute_color_recursive_kernel(ray, scene, color, 0);
}

void compute_color_iterative(ray_t *ray, scene_t *scene, vec3 *color) {
    vec3 finalColor;
    vec3_set(&finalColor, 1, 1, 1);

    size_t depth = 0;
    ray_t inputRay = *ray;

    while (depth < MAX_DEPTH) {
        hit_record_t rec;

        bool b = ray_hit_spheres(&inputRay, scene->objects, scene->n, 0.001f, FLT_MAX, &rec);

        if (!b) {
            break;
        }

        ray_t scattered;
        vec3 attenuation;

        b = material_scatter(&rec.material, &inputRay, &rec, &attenuation, &scattered);

        if (b) {
            vec3_mul_vec(&finalColor, &attenuation, &finalColor);
            inputRay = scattered;
        } else {
            vec3_set(&finalColor, 0, 0, 0);
            break;
        }

        depth += 1;
    }

    vec3 skybox;
    compute_skybox_color(&inputRay, &skybox);

    vec3_mul_vec(&finalColor, &skybox, color);
}

static bool _log_ray_trace = false;

void ray_tracer_enable_logging(bool enabled) {
    _log_ray_trace = enabled;
}

void ray_tracer_render(camera_t *camera, scene_t *scene, int32_t samples, img_t *img) {
    if (_log_ray_trace) {
        fprintf(stdout, "Processing start.\n");
    }

    int32_t width, height;

    img_get_size(img, &width, &height);

    uint64_t timeStart = time_ms();

    ray_t ray;
    vec3 color;

    for (int32_t j = 0; j < height; j += 1) {
        for (int32_t i = 0; i < width; i += 1) {
            vec3 c;
            vec3_set(&c, 0, 0, 0);

            for (int32_t s = 0; s < samples; s += 1) {
                float u = (i + frand()) / (float)width;
                float v = (j + frand()) / (float)height;

                camera_get_ray(camera, u, v, &ray);
                compute_color_iterative(&ray, scene, &c);

                vec3_add(&color, &c, &color);
            }

            vec3_div(&color, (float)samples, &color);

            correct_gamma(&color, 2.0f, &color);

            img_set_pixel(img, i, j, &color);
        }

        if (_log_ray_trace) {
            uint64_t t = time_ms();
            float elapsed = (t - timeStart) / 1000.0f;
            float remaining = elapsed * (height - j - 1) / (j + 1);
            fprintf(stdout, "Processed %d / %d rows; elapsed: %.3f sec; ETA: %.3f sec (%.2f min)\n",
                    j + 1, height, elapsed, remaining, remaining / 60);
        }
    }

    uint64_t timeEnd = time_ms();

    if (_log_ray_trace) {
        fprintf(stdout, "Ray tracing complete.\n");
    }

    uint64_t sampleCount = ray_get_sample_count();
    float timeInSeconds = (timeEnd - timeStart) / 1000.0f;

    if (_log_ray_trace) {
        // https://computergraphics.stackexchange.com/a/2380
        fprintf(stdout, "Total time: %.3f s (%.2f min), %.3lf sample/s\n",
                timeInSeconds, timeInSeconds / 60, (double)sampleCount / timeInSeconds);
    }
}

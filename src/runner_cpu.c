//
// Created by MIC on 2019-02-19.
//

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "utils.h"
#include "ray.h"
#include "scene.h"
#include "img.h"
#include "raytracer.h"

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

void run_cpu_test() {
    frand_state_t frandState;

    frand_init(&frandState, get_random_seed());
    ray_reset_sample_count();

    int32_t width = 200;
    int32_t height = 100;
    int32_t samples = 100;

    camera_t camera;
    scene_t scene;

    setup_basic_camera(&camera, (float)width / (float)height);
    setup_basic_scene(&scene, &frandState);

    img_t *img = img_create(width, height);

    ray_tracer_enable_logging(true);

    ray_tracer_render(&camera, &scene, samples, &frandState, img);

    FILE *fp = fopen("out.ppm", "w");

    img_output(img, fp);

    fclose(fp);

    img_destroy(img);
}

static void ray_tracer_render(__global const camera_t *camera, __global const scene_t *scene, int32_t samples, frand_state_t *frand_state, img_t *img) {
#if !defined(__IN_OPENCL__)
    ray_tracer_log("Processing start.\n");
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
        if (ray_tracer_logging_enabled()) {
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

    ray_tracer_log("Ray tracing complete.\n");

    int64_t sampleCount = ray_get_sample_count();
    float timeInSeconds = (timeEnd - timeStart) / 1000.0f;

    // https://computergraphics.stackexchange.com/a/2380
    ray_tracer_log("Total time: %.3f s (%.2f min), %.3lf sample/s\n",
                   timeInSeconds, timeInSeconds / 60, (double)sampleCount / timeInSeconds);
#endif
}

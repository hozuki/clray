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

EXTERN_C void run_compute_test() {
    frand_state_t frandState;

    frand_init(&frandState, get_random_seed());
#if RAY_TRACER_ENABLE_DETAILED_LOGGING
    ray_reset_sample_count();
#endif

    int32_t width = 200;
    int32_t height = 100;
    int32_t samples = 100;

    camera_t camera;
    scene_t scene;

    setup_basic_camera(&camera, (float)width / (float)height);
    setup_basic_scene(&scene, &frandState);

    img_t *img = img_create(width, height);

#if RAY_TRACER_ENABLE_DETAILED_LOGGING
    ray_tracer_enable_logging(true);
#endif

    ray_tracer_render(&camera, &scene, samples, &frandState, img);

    FILE *fp = fopen("out.ppm", "w");

    img_output(img, fp);

    fclose(fp);

    img_destroy(img);
}

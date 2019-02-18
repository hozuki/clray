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

int main() {
    sdrand48();
    ray_reset_sample_count();

    scene_t scene;

    int32_t width = 1280;
    int32_t height = 640;
    int32_t samples = 100;

    camera_t camera;

    setup_full_camera(&camera, (float)width / (float)height);
    setup_full_scene(&scene);

    img_t *img = img_create(width, height);

    ray_tracer_enable_logging(true);

    ray_tracer_render(&camera, &scene, samples, img);

    FILE *fp = fopen("out.ppm", "w");

    img_output(img, fp);

    fclose(fp);

    img_destroy(img);

    return 0;
}
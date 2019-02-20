//
// Created by MIC on 2019-02-16.
//

#include <stdarg.h>
#include <float.h>
#include <stdio.h>
#include <stdbool.h>

#include <CL/sycl.hpp>

#include "vec3.h"
#include "ray.h"
#include "hit_record.h"
#include "sphere.h"
#include "utils.h"
#include "scene.h"
#include "img.h"
#include "raytracer.h"

namespace sycl = cl::sycl;

static void compute_skybox_color(const ray_t *ray, vec3 *color) {
    float t = 0.5f * (ray->direction.y + 1.0f);

    vec3 c1, c2;
    vec3_set(&c1, 1.0f, 1.0f, 1.0f);
    vec3_set(&c2, 0.5f, 0.7f, 1.0f);

    *color = vec3_lerp(c1, c2, t);
}

#define MAX_DEPTH (50)

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

EXTERN_C void compute_color_recursive(const ray_t *ray, const scene_t *scene, frand_state_t *frand_state, vec3 *color) {
    compute_color_recursive_kernel(ray, scene, frand_state, color, 0);
}

EXTERN_C void compute_color_iterative(const ray_t *ray, const scene_t *scene, frand_state_t *frand_state, vec3 *color) {
    vec3 finalColor;
    vec3_set(&finalColor, 1, 1, 1);

    size_t depth = 0;
    ray_t scattered;
    ray_t *inputRay = (ray_t *)ray;

    const sphere_t *spheres = scene->objects;
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

#if RAY_TRACER_ENABLE_DETAILED_LOGGING

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

EXTERN_C void ray_tracer_render(const camera_t *camera, const scene_t *scene, int32_t samples, frand_state_t *frand_state, img_t *img) {
#if RAY_TRACER_ENABLE_DETAILED_LOGGING
    if (_log_ray_trace) {
        ray_tracer_log("Processing start.\n");
    }
#endif

    int32_t width, height;

    img_get_size(img, &width, &height);

#if RAY_TRACER_ENABLE_DETAILED_LOGGING
    uint64_t timeStart = time_ms();
#endif

    auto total_work_size = (size_t)width * (size_t)height;

    frand_state_t *frandStates = (frand_state_t *)malloc(total_work_size * sizeof(frand_state_t));

    for (auto i = 0; i < total_work_size; i += 1) {
        frand(frand_state);
        frandStates[i].x = frand_state->x * frand_state->x;
    }

    sycl::buffer<vec3> pixelBuf(img->pixels, sycl::range<1>(total_work_size));
    sycl::buffer<camera_t> cameraBuf(camera, sycl::range<1>(1));
    sycl::buffer<scene_t> sceneBuf(scene, sycl::range<1>(1));
    sycl::buffer<frand_state_t> frandStateBuf(frandStates, sycl::range<1>(total_work_size));

    sycl::device device = sycl::default_selector {}.select_device();
    sycl::queue queue(device, [](sycl::exception_list el) {
        for (auto &ex : el) {
            std::rethrow_exception(ex);
        }
    });

    auto wgroup_size = device.get_info<sycl::info::device::max_work_group_size>();
    if (wgroup_size % 2 != 0) {
        throw std::exception("Work-group size has to be even!");
    }
    auto part_size = wgroup_size * 2;

    auto has_local_mem = device.is_host() ||
                         device.get_info<sycl::info::device::local_mem_type>() != sycl::info::local_mem_type::none;
    auto local_mem_size = device.get_info<sycl::info::device::local_mem_size>();
    if (!has_local_mem || local_mem_size < (wgroup_size * sizeof(int32_t))) {
        throw std::exception("Device doesn't have enough local memory!");
    }

    auto n_wgroups = (total_work_size + part_size - 1) / part_size;

    queue.submit([&](sycl::handler &cgh) {
        auto mem_pixels = pixelBuf.get_access<sycl::access::mode::write>(cgh);

        const auto cam1 = cameraBuf.get_access<sycl::access::mode::read>(cgh);
        const auto sce1 = sceneBuf.get_access<sycl::access::mode::read>(cgh);
        const auto frs = frandStateBuf.get_access<sycl::access::mode::read_write>(cgh);

        cgh.parallel_for<class rt_kernel>(
            sycl::nd_range<1>(n_wgroups * wgroup_size, wgroup_size),
            [=](sycl::nd_item<1> item) {
                const auto workId = item.get_global_id(0);
                const auto j = workId / width;
                const auto i = workId - j * width;

                vec3 &pix = mem_pixels[workId];
                const camera_t &cam = cam1[0];
                const scene_t &sce = sce1[0];
                frand_state_t &rs = frs[workId];

                vec3 color;
                vec3_set(&color, 0, 0, 0);
                vec3 c;
                vec3_set(&c, 0, 0, 0);

                for (int32_t s = 0; s < samples; s += 1) {
                    float u = (i + frand(&rs)) / (float)width;
                    float v = (j + frand(&rs)) / (float)height;
                    ray_t ray;

                    camera_get_ray(&cam, u, v, &rs, &ray);
                    compute_color_iterative(&ray, &sce, &rs, &c);

                    color = vec3_add(color, c);
                }

                color = vec3_div(color, (float)samples);

                color = correct_gamma(color, 2.0f);

                pix = color;
            }
        );

#if RAY_TRACER_ENABLE_DETAILED_LOGGING
        if (_log_ray_trace) {
            uint64_t t = time_ms();
            float elapsed = (t - timeStart) / 1000.0f;
            float remaining = elapsed * (height - j - 1) / (j + 1);
            ray_tracer_log("Processed %d / %d rows; elapsed: %.3f sec; ETA: %.3f sec (%.2f min)\n",
                           j + 1, height, elapsed, remaining, remaining / 60);
        }
#endif
    });

    queue.wait_and_throw();

    free(frandStates);

#if RAY_TRACER_ENABLE_DETAILED_LOGGING
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

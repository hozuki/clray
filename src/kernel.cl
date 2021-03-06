//
// Created by MIC on 2019-02-18.
//

//#pragma OPENCL EXTENSION cl_khr_fp64 : enable

//#include "../kernel/xcl.h"

#include "camera.h"
#include "scene.h"
#include "ray.h"
#include "raytracer.h"
#include "utils.h"

__kernel void rt_kernel(__global const camera_t *camera, __global const scene_t *scene,
                        const int width, const int height, const int samples,
                        __global const frand_state_t *states, __global float3 *colors) {
    size_t workId = get_global_id(0);

    frand_state_t frandState = states[workId];

    ray_t ray;
    float3 color = (float3)(0, 0, 0);

    size_t row = workId / width;
    size_t col = workId - width * row;

    for (int s = 0; s < samples; s += 1) {
        float u = (col + frand(&frandState)) / (float)width;
        float v = (row + frand(&frandState)) / (float)height;
        float3 c;

        camera_get_ray(camera, u, v, &frandState, &ray);
        compute_color_iterative(&ray, scene, &frandState, &c);

        color = color + c;
    }

    color = color / samples;

    color = correct_gamma(color, 2.0f);

    colors[workId] = color;
}

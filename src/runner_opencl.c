//
// Created by MIC on 2019-02-18.
//

#if defined(_WIN32) || defined(WIN32)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "utils.h"
#include "ray.h"
#include "scene.h"
#include "img.h"
#include "raytracer.h"

#include "runner_opencl.h"

#define ARRAY_SIZE(arr) (sizeof((arr))/sizeof((arr)[0]))

static void cl_render(camera_t *camera, scene_t *scene, int32_t samples, img_t *img);

static const char *source_files[] = {
    "../src/kernel.cl",
    "../src/camera.c",
    "../src/img.c",
    "../src/pbr_material.c",
    "../src/ray.c",
    "../src/raytracer.c",
    "../src/scene.c",
    "../src/sphere.c",
    "../src/utils.c",
    "../src/vec2.c",
    "../src/vec3.c",
};

static size_t fsize(FILE *fp) {
    long pos = ftell(fp);
    fseek(fp, 0, SEEK_END);
    long end = ftell(fp);
    fseek(fp, pos, SEEK_SET);

    return (size_t)end;
}

#define MAX_LINE_LEN (0x10000 + 1)
#define MAX_SOURCE_SIZE (0x100000 + 1)

static const char *load_concat_sources(const char *fileNames[], size_t n, size_t *size) {
    char *result = (char *)malloc(MAX_SOURCE_SIZE);
    char *line = (char *)malloc(MAX_LINE_LEN);

    memset(result, 0, MAX_SOURCE_SIZE);
    memset(line, 0, MAX_LINE_LEN);

    for (size_t i = 0; i < n; i += 1) {
        FILE *fp = fopen(fileNames[i], "r");

        while (!feof(fp)) {
            if (fgets(line, MAX_LINE_LEN, fp)) {
                strcat(result, line);
            }
        }

        fclose(fp);
    }

    free(line);

    if (size) {
        *size = strlen(result) + 1;
    }

    return result;
}

void run_opencl_test() {
    int32_t width = 200;
    int32_t height = 100;
    int32_t samples = 100;

    camera_t camera;
    scene_t scene;
    frand_state_t frandState;

    frand_init(&frandState, get_random_seed());

    setup_basic_camera(&camera, (float)width / (float)height);
    setup_basic_scene(&scene, &frandState);

    img_t *img = img_create(width, height);

    ray_tracer_enable_logging(false);

    cl_render(&camera, &scene, samples, img);

    FILE *fp = fopen("out.ppm", "w");

    img_output(img, fp);

    fclose(fp);

    img_destroy(img);
}

static void _log(const char *format, ...) {
    va_list args;

        va_start(args, format);
    vfprintf(stdout, format, args);
        va_end(args);
}

static void _check(cl_int r, const char *message) {
    if (r != CL_SUCCESS) {
        fprintf(stderr, "%s (%d)\n", message, r);
    }
}

static void cl_render(camera_t *camera, scene_t *scene, int32_t samples, img_t *img) {
    int32_t width, height;
    img_get_size(img, &width, &height);

    frand_state_t frandState;
    frandState.x = get_random_seed();

    size_t pixelCount = (size_t)width * (size_t)height;

    _log("Image pixel count: %zu\n", pixelCount);

    frand_state_t *seeds = (frand_state_t *)malloc(sizeof(frand_state_t) * pixelCount);

    for (size_t i = 0; i < pixelCount; i += 1) {
        frand(&frandState);
        seeds[i].x = frandState.x * frandState.x;
    }

#define DEVICE_ID_COUNT (2)
#define PLATFORM_ID_COUNT (2)

    int platformIdIndex = 0;
    int deviceIdIndex = 0;

    cl_platform_id platform_id[PLATFORM_ID_COUNT] = {0};
    cl_device_id device_id[DEVICE_ID_COUNT] = {0};
    cl_uint ret_num_devices;
    cl_uint ret_num_platforms;
    cl_int ret = clGetPlatformIDs(PLATFORM_ID_COUNT, platform_id, &ret_num_platforms);

    _log("Actual num platforms: %u\n", ret_num_platforms);

    ret = clGetDeviceIDs(platform_id[platformIdIndex], CL_DEVICE_TYPE_DEFAULT, DEVICE_ID_COUNT, device_id, &ret_num_devices);

    _log("Actual num devices: %u\n", ret_num_devices);

    char *deviceName = (char *)malloc(0x1000);
    memset(deviceName, 0, 0x1000);

    clGetDeviceInfo(device_id[deviceIdIndex], CL_DEVICE_NAME, 0x1000, deviceName, NULL);
    _log("Running on device: %s\n", deviceName);
    free(deviceName);

    cl_context context = clCreateContext(NULL, 1, &device_id[deviceIdIndex], NULL, NULL, &ret);

    cl_command_queue command_queue = clCreateCommandQueue(context, device_id[deviceIdIndex], 0, &ret);

    cl_mem camera_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(camera_t), NULL, &ret);
    _check(ret, "Create buffer: camera");
    cl_mem scene_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(scene_t), NULL, &ret);
    _check(ret, "Create buffer: scene");
    cl_mem seeds_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY, pixelCount * sizeof(frand_state_t), NULL, &ret);
    _check(ret, "Create buffer: seeds");
    cl_mem colors_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, pixelCount * sizeof(vec3), NULL, &ret);
    _check(ret, "Create buffer: colors");

    ret = clEnqueueWriteBuffer(command_queue, camera_mem_obj, CL_TRUE, 0, sizeof(camera_t), camera, 0, NULL, NULL);
    _check(ret, "Write buffer: camera");
    ret = clEnqueueWriteBuffer(command_queue, scene_mem_obj, CL_TRUE, 0, sizeof(scene_t), scene, 0, NULL, NULL);
    _check(ret, "Write buffer: scene");
    ret = clEnqueueWriteBuffer(command_queue, seeds_mem_obj, CL_TRUE, 0, pixelCount * sizeof(frand_state_t), seeds, 0, NULL, NULL);
    _check(ret, "Write buffer: seeds");

    size_t sourceSize;
    size_t fileCount = ARRAY_SIZE(source_files);
    printf("File count: %zu\n", fileCount);
    const char *sourceText = load_concat_sources(source_files, fileCount, &sourceSize);
    printf("Source size: %zu\n", sourceSize);
//    printf("%s\n", sourceText);

    cl_program program = clCreateProgramWithSource(context, 1, &sourceText, &sourceSize, &ret);

    if (ret != CL_SUCCESS) {
        fprintf(stderr, "E: Create program returned %d\n", ret);
    }

    ret = clBuildProgram(program, 1, &device_id[deviceIdIndex], "-D __IN_OPENCL__ -I ../src", NULL, NULL);

    if (ret != CL_BUILD_SUCCESS) {
        fprintf(stderr, "E: Build program returned %d\n", ret);

        char *buf = (char *)malloc(0x1000);
        clGetProgramBuildInfo(program, device_id[deviceIdIndex], CL_PROGRAM_BUILD_LOG, 0x1000, buf, NULL);
        fprintf(stderr, "%s\n", buf);
        free(buf);
    }

    cl_kernel kernel = clCreateKernel(program, "rt_kernel", &ret);
    _check(ret, "Create kernel");

    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (const void *)&camera_mem_obj);
    ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), (const void *)&scene_mem_obj);
    ret |= clSetKernelArg(kernel, 2, sizeof(int32_t), (const void *)&width);
    ret |= clSetKernelArg(kernel, 3, sizeof(int32_t), (const void *)&height);
    ret |= clSetKernelArg(kernel, 4, sizeof(int32_t), (const void *)&samples);
    ret |= clSetKernelArg(kernel, 5, sizeof(cl_mem), (const void *)&seeds_mem_obj);
    ret |= clSetKernelArg(kernel, 6, sizeof(cl_mem), (const void *)&colors_mem_obj);
    _check(ret, "Set args");

    cl_event kernelDone;
    size_t max_local_item_size;
    clGetKernelWorkGroupInfo(kernel, device_id[deviceIdIndex], CL_KERNEL_WORK_GROUP_SIZE, sizeof(max_local_item_size), &max_local_item_size, NULL);
    size_t nw = (size_t)width;
    size_t local_item_size;
    do {
        local_item_size = nw;
        nw = nw / 2;
    } while (nw > max_local_item_size);
    size_t global_item_size = pixelCount;
    _log("Global size: %zu; local size: %zu; max local: %zu\n", global_item_size, local_item_size, max_local_item_size);
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &kernelDone);
    _check(ret, "Enqueue ND kernel");
    ret = clWaitForEvents(1, &kernelDone);
    _check(ret, "Wait for events");

    ret = clFlush(command_queue);
    _check(ret, "Flush");
    ret = clFinish(command_queue);
    _check(ret, "Finish");

    ret = clEnqueueReadBuffer(command_queue, colors_mem_obj, CL_TRUE, 0, pixelCount * sizeof(vec3), img->pixels, 0, NULL, NULL);
    _check(ret, "Read buffer");

    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(camera_mem_obj);
    ret = clReleaseMemObject(scene_mem_obj);
    ret = clReleaseMemObject(seeds_mem_obj);
    ret = clReleaseMemObject(colors_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    free(seeds);

    free((char *)sourceText);
}

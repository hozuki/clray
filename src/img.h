//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_IMG_H
#define CLRAY_IMG_H

#if !defined(__IN_OPENCL__)

#include <stdint.h>
#include <stdio.h>

#endif

#include "vec3.h"
#include "opencl_compat.h"

typedef struct img_t {
    cl_int width;
    cl_int height;
    vec3 *pixels;
} img_t;

img_t *img_create(int32_t width, int32_t height);

void img_destroy(img_t *img);

void img_set_pixel(img_t *img, int32_t x, int32_t y, vec3 *value);

void img_get_pixel(img_t *img, int32_t x, int32_t y, vec3 *result);

void img_output(img_t *img, FILE *file);

int32_t img_get_index(img_t *img, int32_t x, int32_t y);

void img_get_size(img_t *img, int32_t *width, int32_t *height);

#endif //CLRAY_IMG_H

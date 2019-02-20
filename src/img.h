//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_IMG_H
#define CLRAY_IMG_H

#include <stdint.h>
#include <stdio.h>

#include "compat.h"

#include "vec3.h"

typedef struct img_t {
    int32_t width;
    int32_t height;
    vec3 *pixels;
} img_t;

EXTERN_C img_t *img_create(int32_t width, int32_t height);

EXTERN_C void img_destroy(img_t *img);

EXTERN_C void img_set_pixel(img_t *img, int32_t x, int32_t y, vec3 *value);

EXTERN_C void img_get_pixel(img_t *img, int32_t x, int32_t y, vec3 *result);

EXTERN_C void img_output(img_t *img, FILE *file);

EXTERN_C int32_t img_get_index(img_t *img, int32_t x, int32_t y);

EXTERN_C void img_get_size(img_t *img, int32_t *width, int32_t *height);

#endif //CLRAY_IMG_H

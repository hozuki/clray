//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_IMG_H
#define CLRAY_IMG_H

#include <stdint.h>
#include <stdio.h>

struct vec3_t;

typedef struct img_t {
    int32_t width;
    int32_t height;
    struct vec3_t *pixels;
} img_t;

img_t *img_create(int32_t width, int32_t height);

void img_destroy(img_t *img);

void img_set_pixel(img_t *img, int32_t x, int32_t y, struct vec3_t *value);

void img_get_pixel(img_t *img, int32_t x, int32_t y, struct vec3_t *result);

void img_output(img_t *img, FILE *file);

void img_get_size(img_t *img, int32_t *width, int32_t *height);

#endif //CLRAY_IMG_H

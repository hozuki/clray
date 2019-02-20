//
// Created by MIC on 2019-02-16.
//

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "img.h"

EXTERN_C int32_t img_get_index(img_t *img, int32_t x, int32_t y) {
    return x + img->width * y;
}

EXTERN_C img_t *img_create(int32_t width, int32_t height) {
    size_t bufferSize = sizeof(vec3) * width * height;
    vec3 *buffer = (vec3 *)malloc(bufferSize);

    memset(buffer, 0, bufferSize);

    img_t *img = (img_t *)malloc(sizeof(img_t));

    img->width = width;
    img->height = height;
    img->pixels = buffer;

    return img;
}

EXTERN_C void img_destroy(img_t *img) {
    if (img) {
        free(img->pixels);
    }

    free(img);
}

EXTERN_C void img_set_pixel(img_t *img, int32_t x, int32_t y, vec3 *value) {
    int index = img_get_index(img, x, y);
    img->pixels[index] = *value;
}

EXTERN_C void img_get_pixel(img_t *img, int32_t x, int32_t y, vec3 *result) {
    int index = img_get_index(img, x, y);
    *result = img->pixels[index];
}

EXTERN_C void img_get_size(img_t *img, int32_t *width, int32_t *height) {
    if (width) {
        *width = img->width;
    }

    if (height) {
        *height = img->height;
    }
}

EXTERN_C void img_output(img_t *img, FILE *file) {
    fprintf(file, "P3\n");
    fprintf(file, "%d %d\n", img->width, img->height);
    fprintf(file, "255\n");

    for (int32_t j = img->height - 1; j >= 0; j -= 1) {
        for (int32_t i = 0; i < img->width; i += 1) {
            int32_t index = img_get_index(img, i, j);
            vec3 *pixel = img->pixels + index;

            int32_t ir = (int32_t)(255.99f * CLAMP(pixel->x, 0, 1));
            int32_t ig = (int32_t)(255.99f * CLAMP(pixel->y, 0, 1));
            int32_t ib = (int32_t)(255.99f * CLAMP(pixel->z, 0, 1));

            fprintf(file, "%d %d %d", ir, ig, ib);

            if (i < img->width - 1) {
                fprintf(file, "  ");
            }
        }

        fprintf(file, "\n");
    }
}

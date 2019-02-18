//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_UTILS_H
#define CLRAY_UTILS_H

#include <stdint.h>

#define CLAMP(v, min, max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))

struct vec3_t;
struct vec2_t;

void random_in_unit_sphere(struct vec3_t *result);

void random_in_unit_disk(struct vec2_t *result);

void correct_gamma(struct vec3_t *color, float gamma, struct vec3_t *result);

float frand();

void sdrand48();

uint64_t time_ms();

#ifdef _MSC_VER

double drand48();

#endif

#endif //CLRAY_UTILS_H

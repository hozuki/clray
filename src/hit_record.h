//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_HIT_RECORD_H
#define CLRAY_HIT_RECORD_H

#include "vec3.h"
#include "pbr_material.h"

typedef struct hit_record_t {
    float t;
    vec3 hit_point;
    vec3 normal;

    pbr_material_t material;
} hit_record_t;

#endif //CLRAY_HIT_RECORD_H

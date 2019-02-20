//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_PBR_MATERIAL_H
#define CLRAY_PBR_MATERIAL_H

#include <stdbool.h>

#include "compat.h"
#include "vec3.h"

struct ray_t;
struct hit_record_t;
struct frand_state_t;

enum pbr_material_type_e {
    // Solid (diffuse, metal)
        material_type_solid = 0,
    // Transparent (dielectric)
        material_type_refractive = 1,
};

typedef enum pbr_material_type_e pbr_material_type;

typedef struct pbr_material_t {
    vec3 albedo;

    pbr_material_type type;

    // How metallic it is.
    // 0 = pure diffuse
    // 1 = pure metal
    float metallic;
    // How rough it is.
    // 0 = smooth
    // 1 = rough
    float roughness;

    float refractive_index;
} pbr_material_t;

EXTERN_C bool material_scatter(const pbr_material_t *material, const struct ray_t *in, const struct hit_record_t *rec, struct frand_state_t *frand_state, vec3 *attenuation, struct ray_t *scattered);

#endif //CLRAY_PBR_MATERIAL_H

//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_PBR_MATERIAL_H
#define CLRAY_PBR_MATERIAL_H

#if !defined(__IN_OPENCL__)

#include <stdbool.h>

#endif

#include "opencl_compat.h"
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

    // pbr_material_type
    cl_int type;

    // How metallic it is.
    // 0 = pure diffuse
    // 1 = pure metal
    // Only enabled when the material is solid.
    cl_float metallic;
    // How rough it is.
    // 0 = smooth
    // 1 = rough
    // Only enabled when the material is solid.
    cl_float roughness;

    // Refractive index (> 0)
    // Only enabled when the material is refractive.
    cl_float refractive_index;

    // Attenuation rate when the ray travels inside a material (> 0)
    // Only enabled when the material is refractive.
    // Larger value causes photon energy to drop more quickly.
    cl_float path_attenuation_rate;

    cl_int _pad[3];
} pbr_material_t;

bool material_scatter(const pbr_material_t *material, const struct ray_t *in, const struct hit_record_t *rec, struct frand_state_t *frand_state, vec3 *attenuation, struct ray_t *scattered);

#endif //CLRAY_PBR_MATERIAL_H

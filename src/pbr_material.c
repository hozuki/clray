//
// Created by MIC on 2019-02-16.
//

#include <stdlib.h>
#include <math.h>

#include "ray.h"
#include "hit_record.h"
#include "utils.h"
#include "pbr_material.h"

static bool refract(vec3 *direction, vec3 *normal, float niOverNt, vec3 *refracted) {
    float dt = vec3_dot(direction, normal);
    float discriminant = 1 - niOverNt * niOverNt * (1 - dt * dt);

    if (discriminant > 0) {
        vec3 v1, v2;

        vec3_mul(normal, -dt, &v1);
        vec3_add(direction, &v1, &v1);
        vec3_mul(&v1, niOverNt, &v1);

        vec3_mul(normal, sqrtf(discriminant), &v2);

        vec3_sub(&v1, &v2, refracted);

        return true;
    } else {
        return false;
    }
}

static float schlick(float cosine, float refractiveIndex) {
    float r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * powf(1 - cosine, 5);
}

static bool material_scatter_lambertian(pbr_material_t *material, ray_t *in, hit_record_t *rec, vec3 *attenuation, ray_t *scattered) {
    vec3 random;
    random_in_unit_sphere(&random);

    vec3 target;
    vec3_add(&rec->hit_point, &rec->normal, &target);
    vec3_add(&target, &random, &target);

    vec3 newDirection;
    vec3_sub(&target, &rec->hit_point, &newDirection);

    ray_set(scattered, &rec->hit_point, &newDirection);

    vec3_copy(&material->albedo, attenuation);

    return true;
}

static bool material_scatter_metal(pbr_material_t *material, ray_t *in, hit_record_t *rec, vec3 *attenuation, ray_t *scattered) {
    vec3 reflected;
    vec3_reflect(&in->direction, &rec->normal, &reflected);

    vec3 fuzzyOffset;
    random_in_unit_sphere(&fuzzyOffset);
    vec3_mul(&fuzzyOffset, material->roughness, &fuzzyOffset);

    vec3 newDirection;
    vec3_add(&reflected, &fuzzyOffset, &newDirection);

    ray_set(scattered, &rec->hit_point, &newDirection);

    vec3_copy(&material->albedo, attenuation);

    float outAngle = vec3_dot(&scattered->direction, &rec->normal);

    return outAngle > 0;
}

bool material_scatter_dielectric(pbr_material_t *material, ray_t *in, hit_record_t *rec, vec3 *attenuation, ray_t *scattered) {
    vec3_copy(&material->albedo, attenuation);

    float niOverNt;
    vec3 outwardNormal;
    float cosine = vec3_dot(&in->direction, &rec->normal);

    if (cosine > 0) {
        vec3_negate(&rec->normal, &outwardNormal);
        niOverNt = material->refractive_index;
        cosine = material->refractive_index * cosine;
    } else {
        vec3_copy(&rec->normal, &outwardNormal);
        niOverNt = 1 / material->refractive_index;
        cosine = -cosine;
    }

    vec3 refracted;
    float reflectionProbability;

    if (refract(&in->direction, &outwardNormal, niOverNt, &refracted)) {
        reflectionProbability = schlick(cosine, material->refractive_index);
    } else {
        reflectionProbability = 1.0f;
    }

    if (frand() < reflectionProbability) {
        vec3 reflected;
        vec3_reflect(&in->direction, &rec->normal, &reflected);

        ray_set(scattered, &rec->hit_point, &reflected);
    } else {
        ray_set(scattered, &rec->hit_point, &refracted);
    }

    return true;
}

bool material_scatter(pbr_material_t *material, ray_t *in, hit_record_t *rec, vec3 *attenuation, ray_t *scattered) {
    switch (material->type) {
        case material_type_solid: {
            if (frand() < material->metallic) {
                return material_scatter_metal(material, in, rec, attenuation, scattered);
            } else {
                return material_scatter_lambertian(material, in, rec, attenuation, scattered);
            }
        }
        case material_type_refractive: {
            return material_scatter_dielectric(material, in, rec, attenuation, scattered);
        }
        default: {
            return false;
        }
    }
}

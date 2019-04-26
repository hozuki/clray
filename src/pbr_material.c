//
// Created by MIC on 2019-02-16.
//

#if !defined(__IN_OPENCL__)

#include <math.h>

#endif

#if defined(__IN_OPENCL__)

//#define powf powr
#define powf pow
#define sqrtf sqrt

#endif

#include "ray.h"
#include "hit_record.h"
#include "utils.h"
#include "pbr_material.h"

static bool refract(const vec3 *direction, const vec3 *normal, float niOverNt, vec3 *refracted) {
    float dt = vec3_dot(*direction, *normal);
    float discriminant = 1 - niOverNt * niOverNt * (1 - dt * dt);

    if (discriminant > 0) {
        vec3 v1 = vec3_mul(vec3_sub(*direction, vec3_mul(*normal, dt)), niOverNt);
        vec3 v2 = vec3_mul(*normal, sqrtf(discriminant));

        *refracted = vec3_sub(v1, v2);

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

static bool material_scatter_lambertian(const pbr_material_t *material, const ray_t *in, const hit_record_t *rec, frand_state_t *frand_state, vec3 *attenuation, ray_t *scattered) {
    vec3 random = random_in_unit_sphere(frand_state);

    vec3 target = vec3_add(vec3_add(rec->hit_point, rec->normal), random);

    vec3 newDirection = vec3_sub(target, rec->hit_point);

    ray_set(scattered, &rec->hit_point, &newDirection);

    *attenuation = material->albedo;

    return true;
}

static bool material_scatter_metal(const pbr_material_t *material, const ray_t *in, const hit_record_t *rec, frand_state_t *frand_state, vec3 *attenuation, ray_t *scattered) {
    vec3 reflected = vec3_reflect(in->direction, rec->normal);

    vec3 fuzzyOffset = random_in_unit_sphere(frand_state);
    fuzzyOffset = vec3_mul(fuzzyOffset, material->roughness);

    vec3 newDirection = vec3_add(reflected, fuzzyOffset);

    ray_set(scattered, &rec->hit_point, &newDirection);

    *attenuation = material->albedo;

    float outAngle = vec3_dot(scattered->direction, rec->normal);

    return outAngle > 0;
}

static bool material_scatter_dielectric(const pbr_material_t *material, const ray_t *in, const hit_record_t *rec, frand_state_t *frand_state, vec3 *attenuation, ray_t *scattered) {
    float niOverNt;
    vec3 outwardNormal;
    float cosine = vec3_dot(in->direction, rec->normal);
    bool leavingObject = cosine > 0;

    if (leavingObject) {
        // The ray goes from object to environment
        outwardNormal = vec3_negate(rec->normal);
        niOverNt = material->refractive_index;
        cosine = material->refractive_index * cosine;
    } else {
        // The ray goes from environment to object
        outwardNormal = rec->normal;
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

    if (frand(frand_state) < reflectionProbability) {
        vec3 reflected = vec3_reflect(in->direction, rec->normal);

        *attenuation = material->albedo;

        ray_set(scattered, &rec->hit_point, &reflected);
    } else {
        if (leavingObject) {
            vec3 path = vec3_sub(rec->hit_point, in->origin);
            float pathLength = vec3_length(path);
            float power = powf(material->path_attenuation_rate, 1.25f * pathLength);

            *attenuation = vec3_pow(material->albedo, power);
        } else {
            *attenuation = material->albedo;
        }

        ray_set(scattered, &rec->hit_point, &refracted);
    }

    return true;
}

bool material_scatter(const pbr_material_t *material, const ray_t *in, const hit_record_t *rec, frand_state_t *frand_state, vec3 *attenuation, ray_t *scattered) {
    switch (material->type) {
        case material_type_solid: {
            if (frand(frand_state) < material->metallic) {
                return material_scatter_metal(material, in, rec, frand_state, attenuation, scattered);
            } else {
                return material_scatter_lambertian(material, in, rec, frand_state, attenuation, scattered);
            }
        }
        case material_type_refractive: {
            return material_scatter_dielectric(material, in, rec, frand_state, attenuation, scattered);
        }
        default: {
            return false;
        }
    }
}

//
// Created by MIC on 2019-02-16.
//

#include "vec3.h"
#include "sphere.h"
#include "pbr_material.h"
#include "scene.h"

void setup_full_camera(camera_t *camera, float aspect) {
    vec3 eye, lookAt, up;

    vec3_set(&eye, 8, 1.2f, 3);
    vec3_set(&lookAt, -8, 1, -3);
    vec3_set(&up, 0, 1, 0);

    vec3 p;
    vec3_sub(&eye, &lookAt, &p);
    float dist = vec3_length(&p);

    camera_set(camera, &eye, &lookAt, &up, 30.0f, aspect, 0.05f, dist);
}

void setup_test_camera(camera_t *camera, float aspect) {
    vec3 eye, lookAt, up;

    vec3_set(&eye, 2, 0.25f, 1);
    vec3_set(&lookAt, 0, 0, -1);
    vec3_set(&up, 0, 1, 0);

    vec3 p;
    vec3_sub(&eye, &lookAt, &p);
    float dist = vec3_length(&p);

    camera_set(camera, &eye, &lookAt, &up, 30.0f, aspect, 0.01f, dist);
}

void setup_basic_camera(camera_t *camera, float aspect) {
    vec3 eye, lookAt, up;

    vec3_set(&eye, 0, 0, 2);
    vec3_set(&lookAt, 0, 0, 0);
    vec3_set(&up, 0, 1, 0);

    vec3 p;
    vec3_sub(&eye, &lookAt, &p);
    float dist = vec3_length(&p);

    camera_set(camera, &eye, &lookAt, &up, 30.0f, aspect, 0.01f, dist);
}

void setup_full_scene(scene_t *scene) {
    int32_t maxNumSpheres = 500;
    size_t i;
    sphere_t *s;

    sphere_t *pObjects = (sphere_t *)malloc(maxNumSpheres * sizeof(sphere_t));
    scene->objects = pObjects;

    i = 0;
    s = pObjects + i;

    vec3_set(&s->center, 0, -1000, 0);
    s->radius = 1000;
    s->material.type = material_type_solid;
    s->material.metallic = 0;
    vec3_set(&s->material.albedo, 0.5f, 0.5f, 0.5f);
    i += 1;

    vec3 sceneCenter;
    vec3_set(&sceneCenter, 4.0f, 0.2f, 0.0f);

    for (int32_t a = -11; a < 11; a += 1) {
        for (int32_t b = -11; b < 11; b += 1) {
            vec3 center;
            vec3_set(&center, a + 0.9f * frand(), 0.2f, b + 0.9f * frand());

            vec3 tmp;
            vec3_sub(&center, &sceneCenter, &tmp);

            if (vec3_length(&tmp) > 0.9f) {
                s = pObjects + i;

                s->center = center;
                s->radius = 0.2f;

                float chooseMat = frand();

                if (chooseMat < 0.2f) {
                    // Diffuse
                    vec3 albedo;
                    vec3_set(&albedo, frand() * frand(), frand() * frand(), frand() * frand());

                    s->material.type = material_type_solid;
                    s->material.albedo = albedo;
                    s->material.metallic = 0;
                } else if (chooseMat < 0.4f) {
                    // Metallic
                    vec3 albedo;
                    vec3_set(&albedo, 0.5f * (1 + frand()), 0.5f * (1 + frand()), 0.5f * (1 + frand()));

                    s->material.type = material_type_solid;
                    s->material.albedo = albedo;
                    s->material.metallic = 1;
                    s->material.roughness = 0.5f * frand();
                } else {
                    // Glass
                    vec3 albedo;
                    vec3_set(&albedo, 0.5f * (1 + frand()), 0.5f * (1 + frand()), 0.5f * (1 + frand()));

                    s->material.type = material_type_refractive;
                    s->material.albedo = albedo;
                    // [1.0, 2.5)
                    s->material.refractive_index = 1 + frand() * 1.5f;
                }

                i += 1;
            }
        }
    }

    {
        s = pObjects + i;

        vec3_set(&s->center, 0, 1, 0);
        s->radius = 1.0f;
        s->material.type = material_type_refractive;
        vec3_set(&s->material.albedo, 1.0f, 1.0f, 1.0f);
        s->material.refractive_index = 1.5f;

        i += 1;
    }

    {
        s = pObjects + i;

        vec3_set(&s->center, -4, 1, 0);
        s->radius = 1.0f;
        s->material.type = material_type_solid;
        vec3_set(&s->material.albedo, 0.4f, 0.2f, 0.1f);
        s->material.metallic = 0;

        i += 1;
    }

    {
        s = pObjects + i;

        vec3_set(&s->center, 4, 1, 0);
        s->radius = 1.0f;
        s->material.type = material_type_solid;
        vec3_set(&s->material.albedo, 0.7f, 0.6f, 0.5f);
        s->material.metallic = 1;
        s->material.roughness = 0;

        i += 1;
    }

    scene->n = i;
}

void setup_test_scene(scene_t *scene) {
    scene->n = 4;

    sphere_t *pObjects = (sphere_t *)malloc(scene->n * sizeof(sphere_t));
    scene->objects = pObjects;

    sphere_t *s;

    s = pObjects + 0;

    vec3_set(&s->center, 0, 0, -1);
    s->radius = 0.5f;
    vec3_set(&s->material.albedo, 0.8f, 0.3f, 0.3f);
    s->material.type = material_type_solid;
    s->material.metallic = 0;

    s = pObjects + 1;

    vec3_set(&s->center, 0, -100.5f, -1);
    s->radius = 100.0f;
    vec3_set(&s->material.albedo, 0.8f, 0.8f, 0.0f);
    s->material.type = material_type_solid;
    s->material.metallic = 0;

    s = pObjects + 2;

    vec3_set(&s->center, 1, 0, -1);
    s->radius = 0.5f;
    vec3_set(&s->material.albedo, 0.8f, 0.6f, 0.2f);
    s->material.type = material_type_solid;
    s->material.metallic = 1;
    s->material.roughness = 0.25f;

    s = pObjects + 3;

    vec3_set(&s->center, -1, 0, -1);
    s->radius = 0.5f;
    vec3_set(&s->material.albedo, 0.8f, 0.8f, 0.8f);
    s->material.type = material_type_refractive;
    s->material.refractive_index = 1.5f;
}

void setup_basic_scene(scene_t *scene) {
    scene->n = 4;

    sphere_t *pObjects = (sphere_t *)malloc(scene->n * sizeof(sphere_t));
    scene->objects = pObjects;

    sphere_t *s;

    s = pObjects + 0;

    vec3_set(&s->center, 0, 0, -1);
    s->radius = 0.5f;
    vec3_set(&s->material.albedo, 0.1f, 0.2f, 0.5f);
    s->material.type = material_type_solid;
    s->material.metallic = 0;

    s = pObjects + 1;

    vec3_set(&s->center, 0, -100.5f, -1);
    s->radius = 100.0f;
    vec3_set(&s->material.albedo, 0.8f, 0.8f, 0.0f);
    s->material.type = material_type_solid;
    s->material.metallic = 0;

    s = pObjects + 2;

    vec3_set(&s->center, 1, 0, -1);
    s->radius = 0.5f;
    vec3_set(&s->material.albedo, 0.8f, 0.6f, 0.2f);
    s->material.type = material_type_solid;
    s->material.metallic = 1;
    s->material.roughness = 0.0f;

    s = pObjects + 3;

    vec3_set(&s->center, -1, 0, -1);
    s->radius = 0.5f;
    vec3_set(&s->material.albedo, 0.8f, 0.8f, 0.8f);
    s->material.type = material_type_refractive;
    s->material.refractive_index = 1.5f;
}

//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_SCENE_H
#define CLRAY_SCENE_H

#if !defined(__IN_OPENCL__)

#include <stdint.h>

#endif

struct camera_t;

struct frand_state_t;

#define CLRAY_MAX_SCENE_SPHERES (15)

#include "sphere.h"

typedef struct scene_t {
    size_t n;
    sphere_t objects[CLRAY_MAX_SCENE_SPHERES];
} scene_t;

void setup_full_camera(struct camera_t *camera, float aspect);

void setup_test_camera(struct camera_t *camera, float aspect);

void setup_basic_camera(struct camera_t *camera, float aspect);

void setup_full_scene(scene_t *scene, struct frand_state_t *frand_state);

void setup_test_scene(scene_t *scene, struct frand_state_t *frand_state);

void setup_basic_scene(scene_t *scene, struct frand_state_t *frand_state);

#endif //CLRAY_SCENES_H

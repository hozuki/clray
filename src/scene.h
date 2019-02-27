//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_SCENE_H
#define CLRAY_SCENE_H

#if !defined(__IN_OPENCL__)

#include <stdint.h>

#endif

#include "opencl_compat.h"

struct camera_t;

struct frand_state_t;

#define CLRAY_MAX_SCENE_SPHERES (500)

#include "sphere.h"

typedef struct scene_t {
    cl_uint n;
    cl_int _pad0, _pad1, _pad2;
    sphere_t objects[CLRAY_MAX_SCENE_SPHERES];
} scene_t;

void setup_full_camera(struct camera_t *camera, float aspect);

void setup_test_camera(struct camera_t *camera, float aspect);

void setup_basic_camera(struct camera_t *camera, float aspect);

void setup_full_scene(scene_t *scene, struct frand_state_t *frand_state);

void setup_test_scene(scene_t *scene, struct frand_state_t *frand_state);

void setup_basic_scene(scene_t *scene, struct frand_state_t *frand_state);

#if !defined(__IN_OPENCL__)

void scene_debug(scene_t *scene);

#endif

#endif //CLRAY_SCENES_H

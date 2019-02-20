//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_SCENE_H
#define CLRAY_SCENE_H

#include <stdint.h>

#include "compat.h"

struct camera_t;

struct frand_state_t;

#define CLRAY_MAX_SCENE_SPHERES (500)

#include "sphere.h"

typedef struct scene_t {
    size_t n;
    sphere_t objects[CLRAY_MAX_SCENE_SPHERES];
} scene_t;

EXTERN_C void setup_full_camera(struct camera_t *camera, float aspect);

EXTERN_C void setup_test_camera(struct camera_t *camera, float aspect);

EXTERN_C void setup_basic_camera(struct camera_t *camera, float aspect);

EXTERN_C void setup_full_scene(scene_t *scene, struct frand_state_t *frand_state);

EXTERN_C void setup_test_scene(scene_t *scene, struct frand_state_t *frand_state);

EXTERN_C void setup_basic_scene(scene_t *scene, struct frand_state_t *frand_state);

#endif //CLRAY_SCENES_H

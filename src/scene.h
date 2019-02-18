//
// Created by MIC on 2019-02-16.
//

#ifndef CLRAY_SCENE_H
#define CLRAY_SCENE_H

#include <stdint.h>

struct camera_t;

struct sphere_t;

typedef struct scene_t {
    size_t n;
    struct sphere_t *objects;
} scene_t;

void setup_full_camera(struct camera_t *camera, float aspect);

void setup_test_camera(struct camera_t *camera, float aspect);

void setup_basic_camera(struct camera_t *camera, float aspect);

void setup_full_scene(scene_t *scene);

void setup_test_scene(scene_t *scene);

void setup_basic_scene(scene_t *scene);

#endif //CLRAY_SCENES_H

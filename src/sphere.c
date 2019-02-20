//
// Created by MIC on 2019-02-16.
//

#include <math.h>

#include "rt_options.h"
#include "ray.h"
#include "hit_record.h"
#include "sphere.h"

EXTERN_C bool ray_hit_sphere(const ray_t *ray, const sphere_t *sphere, float minDist, float maxDist, hit_record_t *rec) {
#if RAY_TRACER_ENABLE_DETAILED_LOGGING
    ray_increment_sample_count();
#endif

    vec3 oc = vec3_sub(ray->origin, sphere->center);

    float a = vec3_dot(ray->direction, ray->direction);
    float b = 2 * vec3_dot(oc, ray->direction);
    float c = vec3_dot(oc, oc) - sphere->radius * sphere->radius;
    float delta = b * b - 4 * a * c;

    // Note: OpenCL fails here
    if (delta > 0) {
        float sqrtDelta = sqrtf(delta);

        float root1 = (-b - sqrtDelta) / (2 * a);

        if (minDist <= root1 && root1 <= maxDist) {
            rec->t = root1;
            rec->hit_point = ray_point_at(ray, rec->t);
            rec->normal = vec3_normalize(vec3_div(vec3_sub(rec->hit_point, sphere->center), sphere->radius));
            rec->material = sphere->material;

            return true;
        }

        float root2 = (-b + sqrtDelta) / (2 * a);

        if (minDist <= root2 && root2 <= maxDist) {
            rec->t = root2;
            rec->hit_point = ray_point_at(ray, rec->t);
            rec->normal = vec3_normalize(vec3_div(vec3_sub(rec->hit_point, sphere->center), sphere->radius));
            rec->material = sphere->material;

            return true;
        }
    }

    return false;
}

EXTERN_C bool ray_hit_spheres(const ray_t *ray, const sphere_t *spheres, size_t n, float minDist, float maxDist, hit_record_t *rec) {
    hit_record_t tempRec;
    bool hitAnything = false;
    float closestSoFar = maxDist;

    for (size_t i = 0; i < n; i += 1) {
        const sphere_t *sphere = spheres + i;

        if (ray_hit_sphere(ray, sphere, minDist, closestSoFar, &tempRec)) {
            hitAnything = true;
            closestSoFar = tempRec.t;
            *rec = tempRec;
        }
    }

    return hitAnything;
}

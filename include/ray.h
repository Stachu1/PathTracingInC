#ifndef ray_H
#define ray_H
#include "vec3.h"

typedef struct {
    vec3_t ori;
    vec3_t dir;
} ray_t;

void ray_set_origin(ray_t *ray, vec3_t *origin);
void ray_set_direction(ray_t *ray, vec3_t *dir);
void ray_info(ray_t *ray);

#endif

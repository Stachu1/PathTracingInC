#include <stdio.h>
#include <stdlib.h>
#include "ray.h"
#include "vec3.h"


void ray_set_origin(ray_t *ray, vec3_t *ori) {
    if (ray != NULL) {
        ray->ori = *ori;
    }
}

void ray_set_direction(ray_t* ray, vec3_t *dir) {
    if (ray != NULL) {
        ray->dir = *dir;
    }
}

void ray_info(ray_t *ray) {
    if (ray != NULL) {
        printf("Origin: (%.2f, %.2f, %.2f)\n", ray->ori.x, ray->ori.y, ray->ori.z);
        printf("Direction: (%.2f, %.2f, %.2f)\n", ray->dir.x, ray->dir.y, ray->dir.z);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include "ray.h"
#include "vec3.h"


void ray_set_origin(ray_t *ray, vec3_t *origin) {
    if (ray != NULL) {
        ray->origin = *origin;
    }
}

void ray_set_direction(ray_t* ray, vec3_t *direction) {
    if (ray != NULL) {
        ray->direction = *direction;
    }
}

void ray_info(ray_t *ray) {
    if (ray != NULL) {
        printf("Origin: (%.2f, %.2f, %.2f)\n", ray->origin.x, ray->origin.y, ray->origin.z);
        printf("Direction: (%.2f, %.2f, %.2f)\n", ray->direction.x, ray->direction.y, ray->direction.z);
    }
}

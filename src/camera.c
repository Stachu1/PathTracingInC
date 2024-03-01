#include <stdio.h>
#include <stdlib.h>
#include "vec3.h"
#include "ray.h"
#include "camera.h"


void camera_set_ray_for_pixel(camera_t *camera, ray_t *ray, int x, int y) {
    double d_ang_hori = camera->fow / (camera->res_x - 1);
    double d_ang_vert = d_ang_hori * (double)camera->res_y/(double)camera->res_x;

    double ax = (0.5 * (camera->res_y-1) - (double)y) * d_ang_vert;
    double az = (0.5 * (camera->res_x-1) - (double)x) * d_ang_hori;
    
    ray_set_origin(ray, &camera->position);
    ray_set_direction(ray, &camera->direction);
    ray->direction = vec3_rotate(ray->direction, ax, 0, az);
}
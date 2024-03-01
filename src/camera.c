#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vec3.h"
#include "ray.h"
#include "camera.h"


#define ROTATION_SPEED 0.05
#define MOVEMENT_SPEED 0.1


void camera_set_ray_for_pixel(camera_t *camera, ray_t *ray, int x, int y) {
    double pixel_angle_x = camera->fow / (camera->res_x - 1);
    double pixel_angle_y = pixel_angle_x * (double)camera->res_y/camera->res_x;

    double ax = (0.5 * (camera->res_y-1) - y) * pixel_angle_y + camera->angle.x;
    double ay = camera->angle.y;
    double az = (0.5 * (camera->res_x-1) - x) * pixel_angle_x + camera->angle.z;

    ray_set_origin(ray, &camera->pos);
    vec3_set(&ray->dir, 0,1,0);
    ray->dir = vec3_rotate(ray->dir, ax, ay, az);
}


int camera_update_pos(camera_t *camera, SDL_Event *event) {
    const uint8_t* keys = SDL_GetKeyboardState(NULL);
    double delta_az = M_PI_2*keys[4] + M_PI*keys[22] - M_PI_2*keys[7];
    if (keys[4] + keys[26] + keys[22] + keys[7]) {
        camera->vel = vec3_rotate(vec3_init(0,1,0), camera->angle.x, camera->angle.y, camera->angle.z + delta_az);
        camera->vel = vec3_scale(camera->vel, MOVEMENT_SPEED);
        camera->pos = vec3_add(camera->pos, camera->vel);
        return true;
    }
    return false;
}
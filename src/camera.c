#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vec3.h"
#include "ray.h"
#include "camera.h"


#define ROTATION_SPEED 0.05
#define MOVEMENT_SPEED 0.1


void camera_set_ray_for_pixel(camera_t *camera, ray_t *ray, int x, int y) {
    double ax = (0.5 * (camera->res_y-1) - y) * camera->pixel_angle_y + camera->angle.x;
    double az = (0.5 * (camera->res_x-1) - x) * camera->pixel_angle_x + camera->angle.z;

    ray_set_origin(ray, &camera->pos);
    vec3_set(&ray->dir, 0,1,0);
    ray->dir = vec3_rotate(ray->dir, ax, camera->angle.y, az);
}


int camera_update_pos(camera_t *camera) {
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

int camera_ipdate_angle(camera_t *camera, SDL_Event *event) {
    static int start_x;
    static int start_y;
    static int d_x;
    static int d_y;
    static bool mouse_pressed = false;
    static bool mouse_down = false;

    if (event->type == SDL_MOUSEBUTTONDOWN && !mouse_down) {
        mouse_pressed = true;
    }
    if (event->type == SDL_MOUSEBUTTONUP && mouse_down) {
        mouse_down = false;
        printf("x: %d y: %d\n", d_x, d_y);
        camera->angle.x -= d_y * camera->pixel_angle_y;
        camera->angle.z -= d_x * camera->pixel_angle_x;
        return 1;  
    }

    if (event->type == SDL_MOUSEMOTION) {
        if (mouse_pressed) {
            start_x = event->motion.x;
            start_y = event->motion.y;
            mouse_pressed = false;
            mouse_down = true;
        }
        if (mouse_down) {
            d_x = start_x - event->motion.x;
            d_y = start_y - event->motion.y;
        }
        
    }
    return 0;
}
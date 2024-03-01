#ifndef cam_H
#define cam_H
#include "SDL2/SDL.h"
#include "vec3.h"
#include "ray.h"


typedef struct {
    int res_x;
    int res_y;
    double fow;
    vec3_t pos;
    vec3_t vel;
    vec3_t angle;
    
} camera_t;

void camera_set_ray_for_pixel(camera_t *cam, ray_t *ray, int x, int y);
int camera_update_pos(camera_t *camera, SDL_Event *event);

#endif
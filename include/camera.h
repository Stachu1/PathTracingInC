#ifndef cam_H
#define cam_H
#include "vec3.h"
#include "ray.h"


typedef struct {
    int res_x;
    int res_y;
    double fow;
    vec3_t position;
    vec3_t direction;
    vec3_t angle;
} camera_t;

void camera_set_ray_for_pixel(camera_t *cam, ray_t *ray, int x, int y);

#endif
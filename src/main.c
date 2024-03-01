#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "vec3.h"
#include "ray.h"
#include "camera.h"
#include "sphere.h"
#include "color.h"
#include "intersection.h"
#include "material.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600



void init(SDL_Window **, SDL_Renderer **, camera_t *);
void window_test(SDL_Renderer *);
double get_discriminant_sphere(ray_t *, sphere_t *);
double get_intersection_distance_sphere(ray_t *r, vec3_t, double);
color_t render_pixel(int, int, camera_t *, sphere_t *);
void update_intersection_info(intersection_t *, ray_t *, sphere_t *, double);

int main(void) {
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    camera_t camera;
    
    // Initialize
    init(&window, &renderer, &camera);

    sphere_t sphere;
    vec3_set(&sphere.pos, 0, 5, 0);
    sphere.radius = 1;
    color_t white = {255, 255, 255};
    material_t material_white = {white};
    sphere.material = &material_white;


    color_t color;
    for (int y = 0; y < camera.res_y; y++) {
        for (int x = 0; x < camera.res_x; x++) {
            color = render_pixel(x, y, &camera, &sphere);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);

    // Main loop
    while (1) {

        


        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}


color_t render_pixel(int x, int y, camera_t *camera, sphere_t *sphere) {
    ray_t ray;
    camera_set_ray_for_pixel(camera, &ray, x, y);

    double discriminant = get_discriminant_sphere(&ray, sphere);
    if (discriminant < 0) {
        return color_init(0,0,0);
    }

    intersection_t info = {false};
    update_intersection_info(&info, &ray, sphere, discriminant);
    double brightness = -vec3_dot(ray.dir, info.normal);

    return color_init(255*brightness, 255*brightness, 255*brightness);
}


void update_intersection_info(intersection_t *info, ray_t *ray, sphere_t *sphere, double discriminant) {
    double dis = get_intersection_distance_sphere(ray, sphere->pos, discriminant);
    if (dis < info->dis || !info->valid) {
        info->valid = true;
        info->dis = dis;
        info->pos = vec3_add(ray->ori, vec3_scale(ray->dir, dis));
        info->normal = vec3_subtract(info->pos, sphere->pos);
        info->material = sphere->material;
    }
}


double get_intersection_distance_sphere(ray_t *ray, vec3_t sphere_pos, double discriminant) {
    double d = - vec3_dot(ray->dir, vec3_subtract(ray->ori, sphere_pos));
    return d - sqrt(discriminant);
}


double get_discriminant_sphere(ray_t *ray, sphere_t *sphere) {
    vec3_t n1 = vec3_subtract(ray->ori, sphere->pos);
    double a = vec3_dot(ray->dir, n1);
    a *= a;
    double b = vec3_distance(ray->ori, sphere->pos);
    b *= b;
    double c = sphere->radius * sphere->radius;
    return a - b + c;
}


void init(SDL_Window **win, SDL_Renderer **rend, camera_t *camera) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, win, rend);
    SDL_SetWindowTitle(*win, "ELO PathTracing");

    camera->res_x = WINDOW_WIDTH;
    camera->res_y = WINDOW_HEIGHT;
    camera->fow = 45.*M_PI/180.;
    vec3_set(&camera->pos, 0,0,0);
    vec3_set(&camera->dir, 0,1,0);
    vec3_set(&camera->ang, 0,0,0);
}


void window_test(SDL_Renderer *rend) {
    for (int y = 0; y < WINDOW_HEIGHT; y++) {
        for (int x = 0; x < WINDOW_WIDTH; x++) {
            int ux = x * 255 / WINDOW_WIDTH;
            int uy = y * 255 / WINDOW_WIDTH;
            SDL_SetRenderDrawColor(rend, 255-ux, ux, uy, 255);
            SDL_RenderDrawPoint(rend, x, y);
        }
    }
    SDL_RenderPresent(rend);
}
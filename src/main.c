#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "vec3.h"
#include "ray.h"
#include "camera.h"
#include "sphere.h"
#include "color.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600



void init(SDL_Window **, SDL_Renderer **, camera_t *);
void window_test(SDL_Renderer *);
double get_discriminant_for_sphere(ray_t *, sphere_t *);
double get_intersection_distance(ray_t *r, vec3_t, double);
color_t render_pixel(int, int, camera_t *, sphere_t *);

int main(void) {
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    camera_t camera;
    
    // Initialize
    init(&window, &renderer, &camera);

    sphere_t sphere;
    vec3_set(&sphere.position, 0, 5, 0);
    sphere.radius = 1;


    for (int y = 0; y < camera.res_y; y++) {
        for (int x = 0; x < camera.res_x; x++) {
            color_t color = render_pixel(x, y, &camera, &sphere);
            if (color.r > 0) {
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
            }
            else {
                int ux = x * 255 / WINDOW_WIDTH;
                int uy = y * 255 / WINDOW_WIDTH;
                SDL_SetRenderDrawColor(renderer, 255-ux, ux, uy, 255);
            }
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
    double discriminant = get_discriminant_for_sphere(&ray, sphere);
    if (discriminant < 0) {
        return color_init(0,0,0);
    }
    return color_init(255, 255,255);
}



double get_intersection_distance(ray_t *ray, vec3_t body_pos, double discriminant) {
    double d = - vec3_dot(ray->direction, vec3_subtract(ray->origin, body_pos));
    return d - sqrt(discriminant);
}


double get_discriminant_for_sphere(ray_t *ray, sphere_t *sphere) {
    vec3_t n1 = vec3_subtract(ray->origin, sphere->position);
    double a = vec3_dot(ray->direction, n1);
    a *= a;
    double b = vec3_distance(ray->origin, sphere->position);
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
    vec3_set(&camera->position, 0,0,0);
    vec3_set(&camera->direction, 0,1,0);
    vec3_set(&camera->angle, 0,0,0);
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
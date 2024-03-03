#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "vec3.h"
#include "ray.h"
#include "camera.h"
#include "sphere.h"
#include "intersection.h"
#include "material.h"
#include "skybox.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600

#define RES_DIVISOR_IN_PERFOMANCE 4
#define MAX_REFLECTIONS 5


void init(SDL_Window **, SDL_Renderer **, camera_t *, double, skybox_t *);
void window_test(SDL_Renderer *);
double get_discriminant_sphere(ray_t *, sphere_t *);
double get_intersection_distance_sphere(ray_t *, vec3_t, double);
vec3_t render_pixel(int, int, camera_t *, skybox_t *, sphere_t[], int, int);
void reflect_ray(ray_t *, intersection_t *);
vec3_t get_diffused_reflection(vec3_t);
vec3_t get_specular_reflection(vec3_t, vec3_t);
void update_intersection_info(intersection_t *, ray_t *, sphere_t *, double);
void render_screen(SDL_Renderer *, camera_t *, vec3_t [], bool, skybox_t *, sphere_t[], int);
uint32_t millis();


int main(void) {
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    camera_t camera;
    skybox_t skybox;
    
    
    // Initialize
    init(&window, &renderer, &camera, 75, &skybox);
    vec3_t* pixels = malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(vec3_t));
    

    // Create scene
    int spheres_count = 3;
    sphere_t spheres[spheres_count];
    vec3_set(&spheres[0].pos, -3, 5, 0);
    vec3_set(&spheres[1].pos, 0, 5, 0);
    vec3_set(&spheres[2].pos, 3, 5, 0);
    spheres[0].radius = 1;
    spheres[1].radius = 1;
    spheres[2].radius = 1;
    material_t material_r = {vec3_init(1,0,0)};
    material_t material_g = {vec3_init(0,1,0)};
    material_t material_b = {vec3_init(0,0,1)};
    spheres[0].material = &material_r;
    spheres[1].material = &material_g;
    spheres[2].material = &material_b;


    // Main loop
    printf("\n");
    uint32_t last_frame = millis();
    uint32_t last_moved = 0;
    while (1) {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT) break;
        
        bool cam_moved = camera_update_pos(&camera) | camera_update_angle(&camera, &event);
        if (cam_moved) last_moved = last_frame;
        if (millis() - last_moved < 500) {
            // Performance mode
            render_screen(renderer, &camera, pixels, true, &skybox, spheres, spheres_count);
        }
        else {
            // Path Tracing
            render_screen(renderer, &camera, pixels, false, &skybox,  spheres, spheres_count);
        }
        SDL_RenderPresent(renderer);
        uint32_t now_ms = millis();
        printf("\rLastFrame: %dms ", now_ms - last_frame);
        fflush(stdout);
        last_frame = now_ms;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}


void render_screen(SDL_Renderer *renderer, camera_t *camera, vec3_t pixels[], bool perfomance_mode, skybox_t *skybox, sphere_t spheres[], int spheres_count) {
    static int iterations = 0;
    perfomance_mode ? iterations = 0 : iterations++;
    double lerp = (iterations-1) ? 1 - 1./(iterations-1) : 0.;

    int res_divisor = perfomance_mode ? RES_DIVISOR_IN_PERFOMANCE : 1;
    int max_reflections = perfomance_mode ? 0 : MAX_REFLECTIONS;
    
    for (int y = 0; y < camera->res_y; y+=res_divisor) {
        for (int x = 0; x < camera->res_x; x+=res_divisor) {
            vec3_t color = render_pixel(x, y, camera, skybox, spheres, spheres_count, max_reflections);    
            
            if (!perfomance_mode) {
                pixels[x + y * camera->res_x] = vec3_lerp(color, pixels[x + y * camera->res_x], lerp);
                continue;
            }

            // Performance mode reneder
            for (int dy = 0; dy < res_divisor; dy++) {
                for (int dx = 0; dx < res_divisor; dx++) {
                    pixels[(x+dx) + (y+dy) * camera->res_x] = color;
                }
            }
        }
    }
    for (int y = 0; y < camera->res_y; y++) {
        for (int x = 0; x < camera->res_x; x++) {
            vec3_t color = pixels[x + y * camera->res_x];
            SDL_SetRenderDrawColor(renderer, (uint8_t)255.*color.x, (uint8_t)255.*color.y, (uint8_t)255.*color.z, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}


vec3_t render_pixel(int x, int y, camera_t *camera, skybox_t *skybox, sphere_t spheres[], int spheres_count, int max_reflections) {
    ray_t ray;
    vec3_t color = vec3_init(1.,1.,1.);
    camera_set_ray_for_pixel(camera, &ray, x, y);

    for (int ref_index = 0; ref_index <= max_reflections; ref_index++) {
        intersection_t info = {false};
        for (int i = 0; i < spheres_count; i++) {
            double discriminant = get_discriminant_sphere(&ray, &spheres[i]);
            if (discriminant < 0) continue;
            
            double d = get_intersection_distance_sphere(&ray, spheres[i].pos, discriminant);
            if (d < 0) continue;
            if (!info.valid || d < info.dis) {
                update_intersection_info(&info, &ray, &spheres[i], discriminant);
                color = vec3_multiply(color, spheres[i].material->color);
                
                // Cos weighted brightness when in performance mode
                if (!max_reflections) color = vec3_scale(color, -vec3_dot(ray.dir, info.normal));
            }
        }
        if (!info.valid) {
            color = vec3_multiply(color, skybox_get_pixel(skybox, ray.dir));
            break;
        }
        reflect_ray(&ray, &info);
    }
    return color;
}


void reflect_ray(ray_t *ray, intersection_t *info) {
    ray->ori = info->pos;
    ray->dir = get_diffused_reflection(info->normal);
}


vec3_t get_specular_reflection(vec3_t vec, vec3_t norm) {
    return vec3_subtract(vec, vec3_scale(norm, 2 * vec3_dot(vec, norm)));
}


vec3_t get_diffused_reflection(vec3_t norm) {
    vec3_t vec = vec3_random();
    if (vec3_dot(vec, norm) < 0) vec = vec3_negate(vec);
    return vec;
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


void init(SDL_Window **win, SDL_Renderer **rend, camera_t *camera, double fow, skybox_t *skybox) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, win, rend);
    SDL_SetWindowTitle(*win, "PathTracing");

    camera->res_x = WINDOW_WIDTH;
    camera->res_y = WINDOW_HEIGHT;
    camera->fow = fow * M_PI/180.;
    vec3_set(&camera->pos, 0,0,0);
    vec3_set(&camera->vel, 0,0,0);
    vec3_set(&camera->angle, 0,0,0);
    camera->pixel_angle_x = camera->fow / (camera->res_x - 1);
    camera->pixel_angle_y = camera->pixel_angle_x * (double)camera->res_y/camera->res_x;

    skybox_init(skybox, "skybox/skybox.bmp", 3072, 1536);
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

uint32_t millis() {
  struct timespec t ;
  clock_gettime(CLOCK_MONOTONIC_RAW, &t) ;
  return t.tv_sec * 1000 + ( t.tv_nsec + 500000 ) / 1000000 ;
}
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "vec3.h"
#include "skybox.h"


void skybox_init(skybox_t* skybox, const char* filename, int width, int height) {
    skybox->filename = filename;
    skybox->width = width;
    skybox->height = height;
    skybox->file = fopen(filename, "rb");
}


vec3_t skybox_get_pixel(skybox_t* skybox, vec3_t vec) {
    double u = 0.5 + atan2(vec.y, vec.x) * M_1_PI * 0.5;
    double v = 0.5 + asin(vec.z) * M_1_PI;
    int x = skybox->width * u;
    int y = skybox->height * v;

    // For BGRA images and 54 bytes of offset
    long offset = y * skybox->width * 4 + x * 4 + 54;
    fseek(skybox->file, offset, SEEK_SET);
    uint8_t pixel[3];
    fread(pixel, sizeof(uint8_t), 3, skybox->file);

    vec3_t color = vec3_init((double)pixel[2]/255,
                            (double)pixel[1]/255,
                            (double)pixel[0]/255);
    return color;
}
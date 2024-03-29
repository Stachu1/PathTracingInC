#ifndef skybox_H
#define skybox_H
#include <stdio.h>
#include <stdlib.h>
#include "vec3.h"


typedef struct {
    const char* filename;
    int width, height;
    FILE* file;
} skybox_t;


void skybox_init(skybox_t* skybox, const char* filename, int width, int height);
vec3_t skybox_get_pixel(skybox_t* skybox, vec3_t);

#endif
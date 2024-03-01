#ifndef intersection_H
#define intersection_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vec3.h"
#include "material.h"


typedef struct {
    bool valid;
    double dis;
    vec3_t pos;
    vec3_t normal;
    material_t *material;
} intersection_t;


#endif
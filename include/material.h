#ifndef material_H
#define material_H
#include <stdio.h>
#include <stdlib.h>
#include "vec3.h"


typedef struct {
    vec3_t color;
    double smoothness;
} material_t;


#endif
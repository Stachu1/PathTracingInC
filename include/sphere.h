#ifndef sphere_H
#define sphere_H
#include <stdio.h>
#include <stdlib.h>
#include "vec3.h"
#include "material.h"


typedef struct {
    vec3_t pos;
    double radius;
    material_t *material;

} sphere_t;


#endif
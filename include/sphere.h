#ifndef sphere_H
#define sphere_H
#include <stdio.h>
#include <stdlib.h>
#include "vec3.h"


typedef struct {
    vec3_t position;
    double radius;
} sphere_t;


#endif
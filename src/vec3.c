#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "vec3.h"

void vec3_info(vec3_t vec) {
    printf("x: %f, y: %f, z: %f\n", vec.x, vec.y, vec.z);
}

void vec3_set(vec3_t *vec, double x, double y, double z) {
    if (vec != NULL) {
        vec->x = x;
        vec->y = y;
        vec->z = z;
    }
}

vec3_t vec3_add(vec3_t vec1, vec3_t vec2) {
    vec1.x += vec2.x;
    vec1.y += vec2.y;
    vec1.z += vec2.z;
    return vec1;
}

vec3_t vec3_subtract(vec3_t vec1, vec3_t vec2) {
    vec1.x -= vec2.x;
    vec1.y -= vec2.y;
    vec1.z -= vec2.z;
    return vec1;
}

vec3_t vec3_multiply(vec3_t vec1, vec3_t vec2) {
    vec1.x *= vec2.x;
    vec1.y *= vec2.y;
    vec1.z *= vec2.z;
    return vec1;
}

vec3_t vec3_scale (vec3_t vec, double val) {
    vec.x *= val;
    vec.y *= val;
    vec.z *= val;
    return vec;
}

vec3_t vec3_negate(vec3_t vec) {
    vec.x = -vec.x;
    vec.y = -vec.y;
    vec.z = -vec.z;
    return vec;
}

double vec3_length(vec3_t vec) {
    return sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
}

vec3_t vec3_normalize(vec3_t vec) {
    double inv_len = 1 / vec3_length(vec);
    vec.x *= inv_len;
    vec.y *= inv_len;
    vec.z *= inv_len;
    return vec;
}

vec3_t vec3_cross(vec3_t vec1, vec3_t vec2) {
    vec3_t vec;
    vec.x = vec1.y * vec2.z - vec1.z * vec2.y;
    vec.y = vec1.z * vec2.x - vec1.x * vec2.z;
    vec.z = vec1.x * vec2.y - vec1.y * vec2.x;
    return vec;
}

double vec3_dot(vec3_t vec1, vec3_t vec2) {
    return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

double vec3_distance(vec3_t vec1, vec3_t vec2) {
    double x = vec1.x - vec2.x;
    double y = vec1.y - vec2.y;
    double z = vec1.z - vec2.z;
    return sqrt(x*x + y*y + z*z);
}

vec3_t vec3_rotate(vec3_t vec, double ax, double ay, double az) {
    if (ax != 0) {
        double y = vec.y, z = vec.z;
        vec.y = y * cos(ax) - z * sin(ax);
        vec.z = y * sin(ax) + z * cos(ax);
    }
    if (ay != 0) {
        double x = vec.x, z = vec.z;
        vec.x = x * cos(ay) + z * sin(ay);
        vec.z = -x * sin(ay) + z * cos(ay);
    }
    if (az != 0) {
        double x = vec.x, y = vec.y;
        vec.x = x * cos(az) - y * sin(az);
        vec.y = x * sin(az) + y * cos(az);
    }
    return vec;
}

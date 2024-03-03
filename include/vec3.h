#ifndef vec3_H
#define vec3_H


typedef struct {
    double x, y, z;
} vec3_t;


void vec3_info(vec3_t vec);
vec3_t vec3_init(double x, double y, double z);
void vec3_set (vec3_t *vec, double x, double y, double z);
int vec3_is_zero(vec3_t vec);
vec3_t vec3_add (vec3_t vec1, vec3_t vec2);
vec3_t vec3_subtract (vec3_t vec1, vec3_t vec2);
vec3_t vec3_multiply (vec3_t vec1, vec3_t vec2);
vec3_t vec3_scale (vec3_t vec, double val);
vec3_t vec3_negate (vec3_t vec);
double vec3_length (vec3_t vec);
vec3_t vec3_normalize(vec3_t vec);
vec3_t vec3_lerp(vec3_t vec1, vec3_t vec2, double lerp);
vec3_t vec3_cross (vec3_t vec1, vec3_t vec2);
double vec3_dot (vec3_t vec1, vec3_t vec2);
double vec3_distance (vec3_t vec1, vec3_t vec2);
vec3_t vec3_rotate (vec3_t vec, double x, double y, double z);
vec3_t vec3_random();

#endif
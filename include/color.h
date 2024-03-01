#ifndef color_H
#define color_H
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    uint8_t r, g, b;
} color_t;


color_t color_init(uint8_t r, uint8_t g, uint8_t b);
void color_set(color_t *color, uint8_t r, uint8_t g, uint8_t b);
color_t color_blend(color_t color1, color_t color2, int weight1, int weight2);


#endif
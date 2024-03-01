#include <stdio.h>
#include <stdlib.h>
#include "color.h"


color_t color_init(uint8_t r, uint8_t g, uint8_t b) {
    color_t color = {r, g, b};
    return color;
}

void color_set(color_t *color, uint8_t r, uint8_t g, uint8_t b) {
    color->r = r;
    color->g = g;
    color->b = b;
}

color_t color_blend(color_t color1, color_t color2, int weight1, int weight2) {
    double inv_weight = 1. / (weight1 + weight2);
    uint8_t r = ((color1.r * weight1 + color2.r * weight2) * inv_weight);
    uint8_t g = ((color1.r * weight1 + color2.r * weight2) * inv_weight);
    uint8_t b = ((color1.r * weight1 + color2.r * weight2) * inv_weight);
    color_t color = {r, g, b};
    return color;
}
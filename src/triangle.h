#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "display.h"
#include "texture.h"
#include "vector.h"
#include <stdint.h>

typedef uint32_t color_t;

typedef struct
{
  int a, b, c;
  tex2_t a_uv, b_uv, c_uv;
  color_t color;
} face_t;

typedef struct
{
  vec4_t points[3];
  tex2_t tex_coords[3];
  color_t color;
  float avg_depth;
} triangle_t;

void draw_triangle(
    int x0, int y0,
    int x1, int y1,
    int x2, int y2,
    uint32_t color);

void draw_filled_triangle(
    int x0, int y0,
    int x1, int y1,
    int x2, int y2,
    color_t color);

void draw_textured_triangle(
    int x0, int y0, float z0, float w0, float u0, float v0,
    int x1, int y1, float z1, float w1, float u1, float v1,
    int x2, int y2, float z2, float w2, float u2, float v2,
    color_t *texture);

vec3_t barycentric_weights(
    vec2_t a,
    vec2_t b,
    vec2_t c,
    vec2_t p);

#endif

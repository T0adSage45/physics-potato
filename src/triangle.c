#include "triangle.h"
#include "display.h"
#include "texture.h"
#include "vector.h"

void int_swap(int *a, int *b) {
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

void float_swap(float *a, float *b) {
  float tmp = *a;
  *a = *b;
  *b = tmp;
}

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) {
  vec2_t ac = vec2_sub(c, a);
  vec2_t ab = vec2_sub(b, a);
  vec2_t ap = vec2_sub(p, a);
  vec2_t pc = vec2_sub(c, p);
  vec2_t pb = vec2_sub(b, p);

  float Ar_llgm = (ac.x * ab.y - ac.y * ab.x);
  float Alpha = (pc.x * pb.y - pc.y * pb.x) / Ar_llgm;
  float Beta = (ac.x * ap.y - ac.y * ap.x) / Ar_llgm;
  float Gamma = 1.0 - Alpha - Beta;

  vec3_t weights = {Alpha, Beta, Gamma};
  return weights;
};

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                   uint32_t color) {
  draw_line(x0, y0, x1, y1, color);
  draw_line(x1, y1, x2, y2, color);
  draw_line(x2, y2, x0, y0, color);
}

void draw_textured_triangle(int x0, int y0, float z0, float w0, float u0,
                            float v0, int x1, int y1, float z1, float w1,
                            float u1, float v1, int x2, int y2, float z2,
                            float w2, float u2, float v2, color_t *texture) {
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&z0, &z1);
    float_swap(&w0, &w1);
    float_swap(&u0, &u1);
    float_swap(&v0, &v1);
  }
  if (y1 > y2) {
    int_swap(&y1, &y2);
    int_swap(&x1, &x2);
    float_swap(&z1, &z2);
    float_swap(&w1, &w2);
    float_swap(&u1, &u2);
    float_swap(&v1, &v2);
  }
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&z0, &z1);
    float_swap(&w0, &w1);
    float_swap(&u0, &u1);
    float_swap(&v0, &v1);
  }

  v0 = 1.0 - v0;
  v1 = 1.0 - v1;
  v2 = 1.0 - v2;

  vec4_t point_a = {x0, y0, z0, w0};
  vec4_t point_b = {x1, y1, z1, w1};
  vec4_t point_c = {x2, y2, z2, w2};
  tex2_t a_uv = {u0, v0};
  tex2_t b_uv = {u1, v1};
  tex2_t c_uv = {u2, v2};

  float inv_slope_1 = 0;
  float inv_slope_2 = 0;

  if (y1 - y0 != 0)
    inv_slope_1 = (float)(x1 - x0) / (float)(y1 - y0);
  if (y2 - y0 != 0)
    inv_slope_2 = (float)(x2 - x0) / (float)(y2 - y0);
  if (y1 - y0 != 0) {
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;

      if (x_end < x_start) {
        int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
      }

      for (int x = x_start; x < x_end; x++) {
        // draw our pixel with the color that comes from the texture
        draw_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
      }
    }
  }

  inv_slope_1 = 0;
  inv_slope_2 = 0;

  if (y2 - y0 != 0)
    inv_slope_1 = (float)(x2 - x1) / (float)(y2 - y1);
  if (y2 - y0 != 0)
    inv_slope_2 = (float)(x2 - x0) / (float)(y2 - y0);

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;

      if (x_end < x_start) {
        int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
      }

      for (int x = x_start; x < x_end; x++) {
        draw_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
      }
    }
  }
};

void draw_filled_triangle(int x0, int y0, float z0, float w0, int x1, int y1,
                          float z1, float w1, int x2, int y2, float z2,
                          float w2, color_t color) {
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&z0, &z1);
    float_swap(&w0, &w1);
  }
  if (y1 > y2) {
    int_swap(&y1, &y2);
    int_swap(&x1, &x2);
    float_swap(&z1, &z2);
    float_swap(&w1, &w2);
  }
  if (y0 > y1) {
    int_swap(&y0, &y1);
    int_swap(&x0, &x1);
    float_swap(&z0, &z1);
    float_swap(&w0, &w1);
  }
  z0 = 1 - z0;
  z1 = 1 - z1;
  z2 = 1 - z2;

  vec4_t point_a = {x0, y0, z0, w0};
  vec4_t point_b = {x1, y1, z1, w1};
  vec4_t point_c = {x2, y2, z2, w2};

  float inv_slope_1 = 0;
  float inv_slope_2 = 0;

  if (y1 - y0 != 0)
    inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
  if (y2 - y0 != 0)
    inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

  if (y1 - y0 != 0) {
    for (int y = y0; y <= y1; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;
      if (x_end < x_start) {
        int_swap(&x_start, &x_end);
      }
      for (int x = x_start; x < x_end; x++) {
        draw_triangle_pixel(x, y, color, point_a, point_b, point_c);
      }
    };
  };
  inv_slope_1 = 0;
  inv_slope_2 = 0;

  if (y2 - y1 != 0)
    inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
  if (y2 - y0 != 0)
    inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

  if (y2 - y1 != 0) {
    for (int y = y1; y <= y2; y++) {
      int x_start = x1 + (y - y1) * inv_slope_1;
      int x_end = x0 + (y - y0) * inv_slope_2;
      if (x_end < x_start) {
        int_swap(&x_start, &x_end);
      }
      for (int x = x_start; x < x_end; x++) {
        draw_triangle_pixel(x, y, color, point_a, point_b, point_c);
      }
    };
  };
};

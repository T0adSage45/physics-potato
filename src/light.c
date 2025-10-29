#include "light.h"
#include "display.h"
#include <stdint.h>

light_t light = {.direction = {0, 0, 1}};

vec3_t light_to_vec3(light_t l) {
  vec3_t v = {l.direction.x, l.direction.y, l.direction.z};
  return v;
};

void Init_blackHole(blackHole_t *bh, color_t color) {
  fill_circle((int)bh->post.x, (int)bh->post.y, (int)bh->radius, color);
};

void init_light_rays(lightRay_t *r, color_t color) {
  int x0 = r->post.x;
  int y0 = r->post.y;
  int x1 = (r->post.x - r->dir.x * 10); // 50 px long
  int y1 = (r->post.y - r->dir.y * 10);
  draw_line(x0, y0, x1, y1, color);
};

uint32_t light_apply_intensity(uint32_t original_color,
                               float percentage_factor) {
  if (percentage_factor < 0) {
    percentage_factor = 0;
  }
  if (percentage_factor > 1) {
    percentage_factor = 1;
  }
  uint32_t a = (original_color & 0xFF000000);
  uint32_t r = (original_color & 0x00FF0000) * percentage_factor;
  uint32_t g = (original_color & 0x0000FF00) * percentage_factor;
  uint32_t b = (original_color & 0x000000FF) * percentage_factor;
  uint32_t new_color =
      a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
  return new_color;
};

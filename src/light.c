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
  // int x1 = (int)(r->post.x + r->dir.x * 2); // 50 px long
  // int y1 = (int)(r->post.y + r->dir.y * 2);

  if (r->trailCount < TRAIL_LENGTH) {
    r->trails[r->trailCount++] = r->post;
  } else {
    for (int i = 1; i < TRAIL_LENGTH; i++) {
      r->trails[i - 1] = r->trails[i];
    }
    r->trails[TRAIL_LENGTH - 1] = r->post;
  }

  for (int i = 1; i < r->trailCount; i++) {
    // draw_line(r->trails[i - 1].x, r->trails[i - 1].y, r->trails[i].x,
    //           r->trails[i].y, color);
    draw_pixel(r->trails[i - 1].x, r->trails[i - 1].y, color);
  }

  const double G = 6.67430e-11;
  const double C = 299792458.0;
  double mass_SA = 8.54e36;
  double r_s = (2.0 * G * mass_SA) / (C * C);
  double mpPixel = r_s / 100;
  double scaled_radius = r_s / mpPixel;

  if (r->post.x < scaled_radius)
    return;

  // draw_line(x0, y0, x1, y1, color);
  draw_pixel(x0, y0, color);
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

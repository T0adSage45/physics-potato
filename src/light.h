#ifndef LIGHT_H
#define LIGHT_H

#define TRAIL_LENGTH 400

#include "display.h"
#include "vector.h"
#include <stdint.h>

typedef struct {
  vec3_t direction;
} light_t;

typedef struct {
  vec2_t post;
  double r;
  double phi;
  vec2_t dir;
  vec2_t trails[TRAIL_LENGTH];
  int trailCount;
} lightRay_t;

typedef struct {
  vec2_t post;
  double mass;
  float radius;
} blackHole_t;

extern light_t light;

void Init_blackHole(blackHole_t *bh, color_t color);
void init_light_rays(lightRay_t *r, color_t color);

vec3_t light_to_vec3(light_t l);
uint32_t light_apply_intensity(uint32_t original_color,
                               float percentage_factor);
#endif

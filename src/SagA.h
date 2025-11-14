#ifndef SAG_H
#define SAG_H

#include "display.h"
#include "vector.h"

typedef struct {
  vec2_t post;      // x,y
  vec2_t dir;       // {0|1 , 0|1}
  vec2_t polar_cod; // r, phi
  vec2_t vel;       // dr , dphi
  float dt;
} lightRay_t;

typedef struct {
  vec2_t post;
  double mass;
  float radius;
} blackHole_t;

void Init_blackHole(blackHole_t *bh, color_t color);
void init_light_rays(lightRay_t *r, color_t color);

#endif

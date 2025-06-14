#ifndef TEXTURE_H
#define TEXTURE_H

#include "display.h"
#include <stdint.h>
typedef struct {
  float u;
  float v;
} tex2_t;

extern int texture_width;
extern int texture_height;
extern color_t *mesh_texture;

void draw_texel(int x, int y, color_t *texture, vec4_t point_a, vec4_t point_b,
                vec4_t point_c, tex2_t a_uv, tex2_t b_uv, tex2_t c_uv);
extern const uint8_t REDBRICK_TEXTURE[];
#endif

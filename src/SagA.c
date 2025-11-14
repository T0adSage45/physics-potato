#include "SagA.h"
#include "display.h"
#include <stdio.h>

void Init_blackHole(blackHole_t *bh, color_t color) {
  fill_circle(bh->post.x, bh->post.y, bh->radius, color);
  draw_circle(bh->post.x, bh->post.y, bh->radius * 1.5,
              0x44FF00FF);                                    // photon disc
  draw_circle(bh->post.x, bh->post.y, bh->radius * 3, color); // ecretion disc

  // printf("position of blackhole: {%5f,%5f,%5f,%5f} \n", bh->post.x,
  // bh->post.y,
  //        bh->radius, bh->radius * 3);
};

void init_light_rays(lightRay_t *r, color_t color) {
  int x0 = r->post.x;
  int y0 = r->post.y;
  draw_rect(x0, y0, 3, 3, color);
};

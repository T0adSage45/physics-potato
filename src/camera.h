#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
typedef struct {
  vec3_t post;
  vec3_t direction;
  vec3_t fr_vel;
  float yaw;
} camera_t;

extern camera_t camera;

#endif

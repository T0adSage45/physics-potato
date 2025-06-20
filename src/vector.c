#include "vector.h"
#include "light.h"
#include <math.h>

float vec2_length(vec2_t v) { return sqrt((v.x * v.x) + (v.y * v.y)); };
float vec3_length(vec3_t v) {
  return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
};

vec2_t vec2_add(vec2_t a, vec2_t b) {
  vec2_t result = {.x = a.x + b.x, .y = a.y + b.y};
  return result;
};
vec2_t vec2_sub(vec2_t a, vec2_t b) {
  vec2_t result = {.x = a.x - b.x, .y = a.y - b.y};
  return result;
};

vec3_t vec3_add(vec3_t a, vec3_t b) {
  vec3_t result = {.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z};
  return result;
};

vec3_t vec3_sub(vec3_t a, vec3_t b) {
  vec3_t result = {.x = a.x - b.x, .y = a.y - b.y, .z = a.z - b.z};
  return result;
};

vec2_t vec2_mul(vec2_t a, float scaler) {
  vec2_t result = {.x = a.x * scaler, .y = a.y * scaler};
  return result;
};

vec3_t vec3_mul(vec3_t a, float scaler) {
  vec3_t result = {.x = a.x * scaler, .y = a.y * scaler, .z = a.z * scaler};
  return result;
};

vec2_t vec2_div(vec2_t a, float scaler) {
  vec2_t result = {.x = a.x / scaler, .y = a.y / scaler};
  return result;
};

vec3_t vec3_div(vec3_t a, float scaler) {
  return (vec3_t){.x = a.x / scaler, .y = a.y / scaler, .z = a.z / scaler};
};

vec3_t vec3_norm(vec3_t a, vec3_t b) {
  vec3_t result = {.x = a.y * b.z - a.z * b.y,
                   .y = a.z * b.x - a.x * b.z,
                   .z = a.x * b.y - a.y * b.x};
  return result;
};

float vec2_dot(vec2_t a, vec2_t b) { return (a.x * b.x) + (a.y * b.y); };

float vec3_dot(vec3_t a, vec3_t b) {
  return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
};

vec3_t vec3_rotate_x(vec3_t v, float angle) {
  vec3_t rotated_vector = {.x = v.x,
                           .y = v.y * cos(angle) - v.z * sin(angle),
                           .z = v.y * sin(angle) + v.z * cos(angle)

  };
  return rotated_vector;
};
vec3_t vec3_rotate_y(vec3_t v, float angle) {
  vec3_t rotated_vector = {.x = v.x * cos(angle) - v.z * sin(angle),
                           .y = v.y,
                           .z = v.x * sin(angle) + v.z * cos(angle)

  };
  return rotated_vector;
};
vec3_t vec3_rotate_z(vec3_t v, float angle) {
  vec3_t rotated_vector = {.x = v.x * cos(angle) - v.y * sin(angle),
                           .y = v.x * sin(angle) + v.y * cos(angle),
                           .z = v.z

  };
  return rotated_vector;
};

void vec3_normalize(vec3_t *v) {
  float len = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
  v->x /= len;
  v->y /= len;
  v->z /= len;
};

vec4_t vec3_to_vec4(vec3_t v) {
  vec4_t result = {v.x, v.y, v.z, 1.0};
  return result;
};

vec3_t vec4_to_vec3(vec4_t v) {
  vec3_t result = {
      v.x,
      v.y,
      v.z,
  };
  return result;
};
vec2_t vec4_to_vec2(vec4_t v) {
  vec2_t result = {
      v.x,
      v.y,
  };
  return result;
};

#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
  float x;
  float y;
} vec2_t;

typedef struct {
  float x;
  float y;
  float z;
} vec3_t;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} vec4_t;

float vec2_length(vec2_t v);
float vec3_length(vec3_t v);

vec2_t vec2_add(vec2_t a, vec2_t b);
vec3_t vec3_add(vec3_t a, vec3_t b);

vec2_t vec2_sub(vec2_t a, vec2_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);

vec2_t vec2_mul(vec2_t a, float scale);
vec3_t vec3_mul(vec3_t a, float scale);

vec2_t vec2_div(vec2_t a, float scale);
vec3_t vec3_div(vec3_t a, float scale);

vec3_t vec3_norm(vec3_t a, vec3_t b);

float vec2_dot(vec2_t a, vec2_t b);
float vec3_dot(vec3_t a, vec3_t b);

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

void vec3_normalize(vec3_t *v);

vec4_t vec3_to_vec4(vec3_t v);
vec3_t vec4_to_vec3(vec4_t v);
vec2_t vec4_to_vec2(vec4_t v);

#endif

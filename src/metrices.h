#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"
typedef struct {
  float m[4][4];
} mat4_t;

mat4_t mat4_EYE(void);
mat4_t mat4_scale_vec3(float sx, float sy, float sz);
mat4_t mat4_tranlate_vec3(float tx, float ty, float tz);
mat4_t mat4_rotate_x(float angle);
mat4_t mat4_rotate_y(float angle);
mat4_t mat4_rotate_z(float angle);
mat4_t mat4_world_matrix(mat4_t Sm, mat4_t R_xm, mat4_t R_ym, mat4_t R_zm,
                         mat4_t Tm);
mat4_t mat4_perspective(float fov, float aspect, float znear, float zfar);
vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);
mat4_t mat4_mul_mat4(mat4_t m0, mat4_t m1);
vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v);
#endif

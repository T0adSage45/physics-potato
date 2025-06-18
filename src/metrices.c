#include "metrices.h"
#include "vector.h"
#include <math.h>

mat4_t mat4_EYE(void) {
  mat4_t m = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
  return m;
}

mat4_t mat4_scale_vec3(float sx, float sy, float sz) {
  mat4_t m = mat4_EYE();
  m.m[0][0] = sx;
  m.m[1][1] = sy;
  m.m[2][2] = sz;
  // m.m[3][3] = sx;
  return m;
};

mat4_t mat4_tranlate_vec3(float tx, float ty, float tz) {
  mat4_t m = mat4_EYE();
  m.m[0][3] = tx;
  m.m[1][3] = ty;
  m.m[2][3] = tz;
  // m.m[3][3] = 1;
  return m;
};

mat4_t mat4_rotate_x(float angle) {
  float c = cos(angle);
  float s = sin(angle);
  mat4_t m = mat4_EYE();
  m.m[1][1] = c;
  m.m[1][2] = -s;
  m.m[2][1] = s;
  m.m[2][2] = c;
  return m;
};
mat4_t mat4_rotate_y(float angle) {
  float c = cos(angle);
  float s = sin(angle);
  mat4_t m = mat4_EYE();
  m.m[0][0] = c;
  m.m[0][2] = s;
  m.m[2][0] = -s;
  m.m[2][2] = c;
  return m;
};
mat4_t mat4_rotate_z(float angle) {
  float c = cos(angle);
  float s = sin(angle);
  mat4_t m = mat4_EYE();
  m.m[0][0] = c;
  m.m[0][1] = -s;
  m.m[1][0] = s;
  m.m[1][1] = c;
  return m;
};

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v) {
  vec4_t result;
  result.x =
      m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
  result.y =
      m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
  result.z =
      m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
  result.w =
      m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

  return result;
}
mat4_t mat4_mul_mat4(mat4_t m0, mat4_t m1) {
  mat4_t m;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      m.m[i][j] = m0.m[i][0] * m1.m[0][j] + m0.m[i][1] * m1.m[1][j] +
                  m0.m[i][2] * m1.m[2][j] + m0.m[i][3] * m1.m[3][j];
    }
  }
  return m;
};

mat4_t mat4_world_matrix(mat4_t Sm, mat4_t R_xm, mat4_t R_ym, mat4_t R_zm,
                         mat4_t Tm) {
  mat4_t m = mat4_EYE();
  m = mat4_mul_mat4(Sm, m);
  m = mat4_mul_mat4(R_xm, m);
  m = mat4_mul_mat4(R_ym, m);
  m = mat4_mul_mat4(R_zm, m);
  m = mat4_mul_mat4(Tm, m);
  return m;
};

mat4_t mat4_perspective(float fov, float aspect, float znear, float zfar) {
  mat4_t m = {{{0}}};
  m.m[0][0] = aspect * (1 / tan(fov / 2));
  m.m[1][1] = 1 / tan(fov / 2);
  m.m[2][2] = zfar / (zfar - znear);
  m.m[2][3] = (-zfar * znear) / (zfar - znear);
  m.m[3][2] = 1.0;
  return m;
};

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v) {
  vec4_t result = mat4_mul_vec4(mat_proj, v);
  if (result.w != 0.00) {
    result.x /= result.w;
    result.y /= result.w;
    result.z /= result.w;
  };
  return result;
};

mat4_t mat4_look_at(vec3_t cam_lense, vec3_t target, vec3_t up) {
  vec3_t z = vec3_sub(target, cam_lense);
  vec3_normalize(&z);
  vec3_t x = vec3_norm(up, z);
  vec3_normalize(&x);
  vec3_t y = vec3_norm(z, x);

  mat4_t view_matrix = {{
      {x.x, x.y, x.z, -vec3_dot(x, cam_lense)},
      {y.x, y.y, y.z, -vec3_dot(y, cam_lense)},
      {z.x, z.y, z.z, -vec3_dot(z, cam_lense)},
      {0, 0, 0, 1},
  }};
  return view_matrix;
}

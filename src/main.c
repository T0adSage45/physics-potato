#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
#include "array.h"
#include "display.h"
#include "light.h"
#include "mesh.h"
#include "metrices.h"
#include "texture.h"
#include "triangle.h"
#include "upng.h"
#include "vector.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

triangle_t *triangle_to_render = NULL;

bool is_running = false;
int prev_frame_time = 0;

vec3_t camera_position = {.x = 0, .y = 0, .z = 0};
mat4_t proj_matrix;

float grid_scale = 1;
double PI = 3.14;

enum cull_method {
  CULL_NONE,
  CULL_BACKFACE,
} cull_method;

enum render_method {
  RENDER_WIRE,
  RENDER_WIRE_VERTEX,
  RENDER_FILL_TRIANGLE,
  RENDER_FILL_TRIANGLE_WIRE,
  RENDER_TEXTURED_TRIANGLE,
  RENDER_TEXTURED_TRIANGLE_WIRE,
} render_method;

void setup(void) {
  render_method = RENDER_FILL_TRIANGLE;
  cull_method = CULL_BACKFACE;
  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  if (!color_buffer) {
    fprintf(stderr, "Error Allocating color_buffer");
    return;
  }

  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);
  z_buffer = (float *)malloc(sizeof(float) * window_width * window_height);
  float FOV = PI / 3;
  float ASPECT = (float)window_height / (float)window_width;
  float ZNEAR = 0.1;
  float ZFAR = 100.0;
  proj_matrix = mat4_perspective(FOV, ASPECT, ZNEAR, ZFAR);

  // mesh_texture = (color_t *)REDBRICK_TEXTURE;
  // texture_height = 64;
  // texture_width = 64;

  load_obj_mesh_data("./assets/drone.obj");
  // load_cube_mesh_data();

  load_texture_data("./assets/drone.png");
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_QUIT:
    is_running = false;
    break;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      is_running = false;
    else if (event.key.keysym.sym == SDLK_1)
      render_method = RENDER_WIRE_VERTEX;
    else if (event.key.keysym.sym == SDLK_2)
      render_method = RENDER_WIRE;
    else if (event.key.keysym.sym == SDLK_3)
      render_method = RENDER_FILL_TRIANGLE;
    else if (event.key.keysym.sym == SDLK_4)
      render_method = RENDER_FILL_TRIANGLE_WIRE;
    else if (event.key.keysym.sym == SDLK_5)
      render_method = RENDER_TEXTURED_TRIANGLE;
    else if (event.key.keysym.sym == SDLK_6)
      render_method = RENDER_TEXTURED_TRIANGLE_WIRE;
    else if (event.key.keysym.sym == SDLK_c)
      cull_method = CULL_BACKFACE;
    else if (event.key.keysym.sym == SDLK_d)
      cull_method = CULL_NONE;
    break;
  }
}

// vec2_t project(vec3_t point) {
//   vec2_t projected_point = {.x = (point.x * POV) / point.z,
//                             .y = (point.y * POV) / point.z};
//   return projected_point;
// };

void update(void) {
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - prev_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  prev_frame_time = SDL_GetTicks();

  triangle_to_render = NULL;

  mesh.rotation.x = 0.5;
  mesh.rotation.y += 0.004;
  // mesh.rotation.z += 0.006;
  mesh.scale.x = 1.5;
  mesh.scale.y = 1.5;
  mesh.scale.z = 1.5;
  mesh.translation.z = 5.0;

  mat4_t scale_matrix =
      mat4_scale_vec3(mesh.scale.x, mesh.scale.y, mesh.scale.z);
  mat4_t translate_matrix = mat4_tranlate_vec3(
      mesh.translation.x, mesh.translation.y, mesh.translation.z);
  mat4_t rotate_x = mat4_rotate_x(mesh.rotation.x);
  mat4_t rotate_y = mat4_rotate_y(mesh.rotation.y);
  mat4_t rotate_z = mat4_rotate_z(mesh.rotation.z);

  int N_FACES = array_length(mesh.faces);
  for (int i = 0; i < N_FACES; i++) {
    face_t current_mesh_face = mesh.faces[i];
    vec3_t face_vertices[3];
    face_vertices[0] = mesh.vertices[current_mesh_face.a];
    face_vertices[1] = mesh.vertices[current_mesh_face.b];
    face_vertices[2] = mesh.vertices[current_mesh_face.c];

    vec4_t transformed_vertices[3];

    for (int j = 0; j < 3; j++) {
      vec4_t transformed_vertex = vec3_to_vec4(face_vertices[j]);

      // mat4_t world_matrix = mat4_world_matrix(scale_matrix, rotate_x,
      // rotate_y,rotate_z, translate_matrix);
      mat4_t world_matrix = mat4_world_matrix(scale_matrix, rotate_x, rotate_y,
                                              rotate_z, translate_matrix);

      transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

      // transformed_vertex = vec3_rotate_x(transformed_vertex,
      // mesh.rotation.x); transformed_vertex =
      // vec3_rotate_y(transformed_vertex, mesh.rotation.y);
      // transformed_vertex = vec3_rotate_z(transformed_vertex,
      // mesh.rotation.z); translate forward so cube is in front of camera

      transformed_vertices[j] = transformed_vertex;
    };

    vec3_t vector_a = vec4_to_vec3(transformed_vertices[0]);
    vec3_t vector_b = vec4_to_vec3(transformed_vertices[1]);
    vec3_t vector_c = vec4_to_vec3(transformed_vertices[2]);

    vec3_t vector_ab = vec3_sub(vector_b, vector_a);
    vec3_t vector_ac = vec3_sub(vector_c, vector_a);
    vec3_normalize(&vector_ab);
    vec3_normalize(&vector_ac);

    vec3_t normal = vec3_norm(vector_ab, vector_ac);
    vec3_normalize(&normal);

    vec3_t cam_ray = vec3_sub(camera_position, vector_a);
    float allingment = vec3_dot(normal, cam_ray);

    if (cull_method == CULL_BACKFACE) {
      if (allingment < 0) {
        continue;
      }
    }
    vec4_t projected_points[3];

    for (int j = 0; j < 3; j++) {
      // projected_points[j] = project(vec4_to_vec3(transformed_vertices[j]));
      projected_points[j] =
          mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);

      projected_points[j].y *= -1;
      projected_points[j].x *= (window_width / 2.0);
      projected_points[j].y *= (window_height / 2.0);
      projected_points[j].x += (window_width / 2.0);
      projected_points[j].y += (window_height / 2.0);
    };

    float light_intensity_factor = -vec3_dot(normal, light.direction);

    uint32_t triangle_color =
        light_apply_intensity(current_mesh_face.color, light_intensity_factor);

    triangle_t projected_triangle = {
        .points =
            {
                {projected_points[0].x, projected_points[0].y,
                 projected_points[0].z, projected_points[0].w},
                {projected_points[1].x, projected_points[1].y,
                 projected_points[1].z, projected_points[1].w},
                {projected_points[2].x, projected_points[2].y,
                 projected_points[2].z, projected_points[2].w},
            },
        .tex_coords =
            {
                {current_mesh_face.a_uv.u, current_mesh_face.a_uv.v},
                {current_mesh_face.b_uv.u, current_mesh_face.b_uv.v},
                {current_mesh_face.c_uv.u, current_mesh_face.c_uv.v},
            },
        .color = triangle_color,
    };
    array_push(triangle_to_render, projected_triangle);
  };
};
void render(void) {
  SDL_RenderClear(renderer);

  int N_TRIANGLES = array_length(triangle_to_render);
  for (int i = 0; i < N_TRIANGLES; i++) {
    triangle_t triangle = triangle_to_render[i];

    if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX ||
        render_method == RENDER_FILL_TRIANGLE_WIRE ||
        render_method == RENDER_TEXTURED_TRIANGLE_WIRE) {
      draw_triangle(triangle.points[0].x, triangle.points[0].y,
                    triangle.points[1].x, triangle.points[1].y,
                    triangle.points[2].x, triangle.points[2].y, 0xFFFFFFFF);
    }
    if (render_method == RENDER_TEXTURED_TRIANGLE ||
        render_method == RENDER_TEXTURED_TRIANGLE_WIRE) {
      draw_textured_triangle(
          triangle.points[0].x, triangle.points[0].y, triangle.points[0].z,
          triangle.points[0].w, triangle.tex_coords[0].u,
          triangle.tex_coords[0].v, triangle.points[1].x, triangle.points[1].y,
          triangle.points[1].z, triangle.points[1].w, triangle.tex_coords[1].u,
          triangle.tex_coords[1].v, triangle.points[2].x, triangle.points[2].y,
          triangle.points[2].z, triangle.points[2].w, triangle.tex_coords[2].u,
          triangle.tex_coords[2].v, mesh_texture);
    }

    if (render_method == RENDER_FILL_TRIANGLE ||
        render_method == RENDER_FILL_TRIANGLE_WIRE) {
      draw_filled_triangle(
          triangle.points[0].x, triangle.points[0].y, triangle.points[0].z,
          triangle.points[0].w, triangle.points[1].x, triangle.points[1].y,
          triangle.points[1].z, triangle.points[1].w, triangle.points[2].x,
          triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
          triangle.color);
    }
    if (render_method == RENDER_WIRE_VERTEX) {
      draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6,
                0x0000FFFF);
      draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6,
                0x00FF00FF);
      draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6,
                0xFF0000FF);
    }
  }

  array_free(triangle_to_render);
  render_color_buffer();
  clear_color_buffer(0xFF000000);
  clear_z_buffer();
  SDL_RenderPresent(renderer);
}

void free_resources(void) {
  free(color_buffer);
  free(z_buffer);
  upng_free(png_texture);
  array_free(mesh.faces);
  array_free(mesh.vertices);
};

int main(void) {
  is_running = initalize_window();
  setup();

  if (!color_buffer || !color_buffer_texture) {
    fprintf(stderr, "Setup failed, exiting.\n");
    return -1;
  }

  while (is_running) {
    process_input();
    update();
    render();
  }

  destroy_window();
  free_resources();
  return 0;
}

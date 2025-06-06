#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_timer.h"
#include "array.h"
#include "display.h"
#include "math.h"
#include "matrices.h"
#include "mesh.h"
#include "triangle.h"
#include "vector.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

triangle_t *triangle_to_render = NULL;

vec3_t camera_position = {.x = 0, .y = 0, .z = 0};

int prev_frame_time = 0;
mat4_t proj_matrix;

bool is_running = false;
float grid_scale = 1;
double PI = 3.141592;

enum render_method render_method = RENDER_WIRE;
enum cull_method cull_method = CULL_BACKFACE;

void setup(void) {

  color_buffer =
      (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);
  if (!color_buffer) {
    fprintf(stderr, "Error Allocating color_buffer");
    return;
  }

  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);
  float FOV = PI / 3;
  float ASPECT = (float)window_height / (float)window_width;
  float ZNEAR = 0.1;
  float ZFAR = 100.0;
  proj_matrix = mat4_perspective(FOV, ASPECT, ZNEAR, ZFAR);
  load_obj_mesh_data("./assets/f22.obj");
  // load_cube_mesh_data();
}

void process_input(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      is_running = false;
      break;

    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_running = false;
      } else if (event.key.keysym.sym == SDLK_1) {
        render_method = RENDER_WIRE_VERTEX;
      } else if (event.key.keysym.sym == SDLK_2) {
        render_method = RENDER_WIRE;
      } else if (event.key.keysym.sym == SDLK_3) {
        render_method = RENDER_FILL_TRIANGLE;
      } else if (event.key.keysym.sym == SDLK_4) {
        render_method = RENDER_FILL_TRIANGLE_WIRE;
      } else if (event.key.keysym.sym == SDLK_c) {
        cull_method = CULL_BACKFACE;
      } else if (event.key.keysym.sym == SDLK_d) {
        cull_method = CULL_NONE;
      } else if ((event.key.keysym.sym == SDLK_EQUALS ||
                  event.key.keysym.sym == SDLK_PLUS) &&
                 (event.key.keysym.mod & KMOD_CTRL)) {
        grid_scale += .5;
      } else if (event.key.keysym.sym == SDLK_MINUS &&
                 (event.key.keysym.mod & KMOD_CTRL)) {
        grid_scale -= .5;
      }
      break;
    }
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

  mesh.rotation.x += 0.0041;
  mesh.rotation.y += 0.0041;
  mesh.rotation.z += 0.009;
  mesh.scale.x += 0.000;
  mesh.scale.y += 0.000;
  mesh.scale.z += 0.000;
  mesh.translation.x += 0.005;
  mesh.translation.y += 0.000;
  mesh.translation.z = -10;

  mat4_t scale_matrix =
      mat4_scale_vec3(mesh.scale.x, mesh.scale.y, mesh.scale.z);
  mat4_t rotate_x = mat4_rotate_x(mesh.rotation.x);
  mat4_t rotate_y = mat4_rotate_y(mesh.rotation.y);
  mat4_t rotate_z = mat4_rotate_z(mesh.rotation.z);
  mat4_t translate_matrix = mat4_tranlate_vec3(
      mesh.translation.x, mesh.translation.y, mesh.translation.z);
  mat4_t world_matrix = mat4_world_matrix(scale_matrix, rotate_x, rotate_y,
                                          rotate_z, translate_matrix);

  int N_FACES = array_length(mesh.faces);
  for (int i = 0; i < N_FACES; i++) {
    face_t current_mesh_face = mesh.faces[i];
    vec3_t face_vertices[3];

    face_vertices[0] = mesh.vertices[current_mesh_face.a - 1];
    face_vertices[1] = mesh.vertices[current_mesh_face.b - 1];
    face_vertices[2] = mesh.vertices[current_mesh_face.c - 1];

    vec4_t transformed_vertices[3];

    for (int j = 0; j < 3; j++) {
      vec4_t transformed_vertex = vec3_to_vec4(face_vertices[j]);

      // mat4_t world_matrix = mat4_world_matrix(scale_matrix, rotate_x,
      // rotate_y,rotate_z, translate_matrix);

      transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

      // transformed_vertex = vec3_rotate_x(transformed_vertex,
      // mesh.rotation.x); transformed_vertex =
      // vec3_rotate_y(transformed_vertex, mesh.rotation.y);
      // transformed_vertex = vec3_rotate_z(transformed_vertex,
      // mesh.rotation.z); translate forward so cube is in front of camera

      transformed_vertices[j] = transformed_vertex;
    };

    if (cull_method == CULL_BACKFACE) {

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

      float allingment = vec3_dot(cam_ray, normal);

      if (allingment <= 0) {
        continue;
      }
    }

    float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z +
                       transformed_vertices[2].z) /
                      3.0;

    vec4_t projected_points[3];

    for (int j = 0; j < 3; j++) {
      // projected_points[j] = project(vec4_to_vec3(transformed_vertices[j]));
      projected_points[j] =
          mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);
      projected_points[j].x *= (window_width / 2.0);
      projected_points[j].y *= (window_height / 2.0);
      projected_points[j].x += (window_width / 2.0);
      projected_points[j].y += (window_height / 2.0);
    };
    triangle_t projected_triangle = {
        .points =
            {
                {projected_points[0].x, projected_points[0].y},
                {projected_points[1].x, projected_points[1].y},
                {projected_points[2].x, projected_points[2].y},
            },
        .color = current_mesh_face.color,
        .avg_depth = avg_depth};
    // triangle_to_render[i] = projected_triangle;
    array_push(triangle_to_render, projected_triangle);
  }

  int num_triangles = array_length(triangle_to_render);
  for (int i = 0; i < num_triangles; i++) {
    for (int j = i; j < num_triangles; j++) {
      if (triangle_to_render[i].avg_depth < triangle_to_render[j].avg_depth) {
        triangle_t temp = triangle_to_render[i];
        triangle_to_render[i] = triangle_to_render[j];
        triangle_to_render[j] = temp;
      }
    }
  }
}

void render(void) {
  // draw_grid(grid_scale, 0xFF00FFFF);

  int N_TRIANGLES = array_length(triangle_to_render);
  for (int i = 0; i < N_TRIANGLES; i++) {
    triangle_t triangle = triangle_to_render[i];
    // draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFF0000);
    // draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFF0000FF);
    // draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFF00FFFF);

    if (render_method == RENDER_FILL_TRIANGLE ||
        render_method == RENDER_FILL_TRIANGLE_WIRE) {
      draw_triangle(triangle.points[0].x, triangle.points[0].y,
                    triangle.points[1].x, triangle.points[1].y,
                    triangle.points[2].x, triangle.points[2].y, 0xFF000095);
    }

    if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX ||
        render_method == RENDER_FILL_TRIANGLE_WIRE) {
      draw_filled_triangle(triangle.points[0].x, triangle.points[0].y,
                           triangle.points[1].x, triangle.points[1].y,
                           triangle.points[2].x, triangle.points[2].y,
                           triangle.color = 0xFF00FFFF);
    }
    if (render_method == RENDER_WIRE_VERTEX) {
      draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6,
                0xFFFFFFFF);
      draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6,
                0xFFFFFFFF);
      draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6,
                0xFFFFFFFF);
    }
  }

  array_free(triangle_to_render);
  render_color_buffer();
  clear_color_buffer(0xFF000000);
  SDL_RenderPresent(renderer);
}

void free_resources(void) {
  free(color_buffer);
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

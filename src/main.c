#include "SDL2/SDL_timer.h"
#include "display.h"
#include "mesh.h"
#include "triangle.h"
#include "vector.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

triangle_t triangle_to_render[N_MESH_FACES];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

float POV = 640;
int prev_frame_time = 0;

bool is_running = false;

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
      }

      // Detect CTRL + PLUS
      else if ((event.key.keysym.sym == SDLK_EQUALS ||
                event.key.keysym.sym == SDLK_PLUS) &&
               (event.key.keysym.mod & KMOD_CTRL)) {
        grid_scale += 10;
      }

      // Detect CTRL + MINUS
      else if (event.key.keysym.sym == SDLK_MINUS &&
               (event.key.keysym.mod & KMOD_CTRL)) {
        grid_scale -= 10;
      }
      break;
    }
  }
}

vec2_t project(vec3_t point) {
  vec2_t projected_point = {.x = (point.x * POV) / point.z,
                            .y = (point.y * POV) / point.z};
  return projected_point;
}

void update(void) {
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - prev_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  prev_frame_time = SDL_GetTicks();

  cube_rotation.x += 0.01;
  cube_rotation.y += 0.01;
  cube_rotation.z += 0.01;

  for (int i = 0; i < N_MESH_FACES; i++) {
    face_t current_mesh_face = mesh_faces[i];
    vec3_t face_vertices[3];

    face_vertices[0] = mesh_vertices[current_mesh_face.a - 1];
    face_vertices[1] = mesh_vertices[current_mesh_face.b - 1];
    face_vertices[2] = mesh_vertices[current_mesh_face.c - 1];

    triangle_t projected_triangle;

    for (int j = 0; j < 3; j++) {
      vec3_t transformed_vertex = face_vertices[j];

      transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
      transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
      transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);

      // translate forward so cube is in front of camera
      transformed_vertex.z -= camera_position.z;

      vec2_t projected_point = project(transformed_vertex);
      projected_point.x += (window_width / 2);
      projected_point.y += (window_height / 2);
      projected_triangle.points[j] = projected_point;
    }

    triangle_to_render[i] = projected_triangle;
  }
}

void render(void) {
  draw_grid();

  for (int i = 0; i < N_MESH_FACES; i++) {
    triangle_t triangle = triangle_to_render[i];
    draw_rect(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFFFF0000);
    draw_rect(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFF0000FF);
    draw_rect(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFF00FFFF);

    draw_triangle(triangle.points[0].x, triangle.points[0].y,
                  triangle.points[1].x, triangle.points[1].y,
                  triangle.points[2].x, triangle.points[2].y, 0xFFFFFF00);
  }

  render_color_buffer();
  clear_color_buffer(0xFF000000);
  SDL_RenderPresent(renderer);
}

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
  return 0;
}

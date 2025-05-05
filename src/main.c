#include "display.h"
#include "vector.h"
#include <stdint.h>
#include <stdio.h>

#define GRID_SIZE_X 9
#define GRID_SIZE_Y 9
#define GRID_SIZE_Z 9

vec3_t camera_posotion = {.x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

float POV = 640;

int N_POINTS = GRID_SIZE_X * GRID_SIZE_Y * GRID_SIZE_Z;
vec3_t cube_points[GRID_SIZE_X * GRID_SIZE_Y * GRID_SIZE_Z];
vec2_t projected_points[GRID_SIZE_X * GRID_SIZE_Y * GRID_SIZE_Z];

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
  int point_count = 0;
  for (float x = -1; x <= 1; x += 0.25) {
    for (float y = -1; y <= 1; y += 0.25) {
      for (float z = -1; z <= 1; z += 0.45) {
        vec3_t new_point = {.x = x, .y = y, .z = z};
        cube_points[point_count++] = new_point;
      }
    }
  }
};

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
};

void update(void) {
  cube_rotation.x += 0.005;
  cube_rotation.y += 0.005;
  cube_rotation.z += 0.005;

  for (int i = 0; i < N_POINTS; i++) {
    vec3_t point = cube_points[i];

    vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
    transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
    transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

    transformed_point.z -= camera_posotion.z;

    vec2_t projected_point = project(transformed_point);

    projected_points[i] = projected_point;
  }
};

void render(void) {
  draw_pixel(10, 50, 0xFF50FFFF);
  draw_grid();
  // draw_rect(500, 200, 800, 400, 0xFFFF00FF);

  for (int i = 0; i < N_POINTS; i++) {
    vec2_t projected_point = projected_points[i];
    draw_rect(projected_point.x + (window_width / 2),
              projected_point.y + (window_height / 2), 4, 4, 0xFFFFFF00);
  };

  render_color_buffer();
  clear_color_buffer(0xFF000000);

  SDL_RenderPresent(renderer);
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
  return 0;
}

#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_video.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_buffer_texture = NULL;
uint32_t *color_buffer = NULL; // declaration of first element of memory.
int window_width = 800;
int window_height = 600;
int grid_scale = 25;

bool initalize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  window_width = display_mode.w;
  window_height = display_mode.h;

  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       window_width, window_height, SDL_WINDOW_BORDERLESS);

  if (!window) {
    fprintf(stderr, "Error Creating SDL window.\n");
    return false;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error Creating the SDL renderer. \n");
    return false;
  }
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  return true;
}

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

void update(void) {

};

void clear_color_buffer(uint32_t color) {

  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++) {
      color_buffer[(window_width * y) + x] = color;
    }
  }
};

void render_texture() {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer,
                    (int)(window_width * sizeof(uint32_t)));
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
};

void draw_grid() {
  if (grid_scale < 10) {
    grid_scale = 10;
  }

  if (grid_scale > 100) {
    grid_scale = 100;
  }

  for (int y = 0; y < window_height; y += grid_scale) {
    for (int x = 0; x < window_width; x += grid_scale) {

      color_buffer[(window_width * y) + x] = 0x50333333;
    }
  }
};

void draw_rect(int x, int y, int width, int height, uint32_t color) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      int current_x = x + i;
      int current_y = y + j;
      color_buffer[(window_width * current_y) + current_x] = color;
    }
  }
};

void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 25, 50);
  SDL_RenderClear(renderer);

  draw_grid();
  draw_rect(500, 200, 800, 400, 0xFFFF00FF);
  render_texture();
  clear_color_buffer(0xFF000000);

  SDL_RenderPresent(renderer);
};

void destroy_window(void) {
  free(color_buffer);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
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

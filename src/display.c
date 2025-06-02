#include "display.h"
#include <stdint.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *color_buffer_texture = NULL;
color_t *color_buffer = NULL; // declaration of first element of memory.
int window_width = 800;
int window_height = 600;

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

void clear_color_buffer(color_t color) {

  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++) {
      color_buffer[(window_width * y) + x] = color;
    }
  }
};

void draw_grid(float grid_scale, color_t color) {
  if (grid_scale < 0) {
    grid_scale = 1;
  }
  for (int y = 0; y < window_height; y += grid_scale) {
    for (int x = 0; x < window_width; x += grid_scale) {

      draw_pixel(x, y, color);
      // color_buffer[(window_width * y) + x] = color;
    }
  }
};

void draw_rect(int x, int y, int width, int height, color_t color) {
  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      int current_x = x + i;
      int current_y = y + j;
      draw_pixel(current_x, current_y, color);
      // color_buffer[(window_width * current_y) + current_x] = color;
    }
  }
};

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                   color_t color) {
  draw_line(x0, y0, x1, y1, color);
  draw_line(x1, y1, x2, y2, color);
  draw_line(x2, y2, x0, y0, color);
};

void draw_line(int x0, int y0, int x1, int y1, color_t color) {
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = (x0 < x1) ? 1 : -1;
  int sy = (y0 < y1) ? 1 : -1;
  int err = dx - dy;

  while (1) {
    draw_pixel(x0, y0, color);
    if (x0 == x1 && y0 == y1)
      break;
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y0 += sy;
    }
  }
};

void draw_pixel(int x, int y, color_t color) {
  if (x >= 0 && x < window_width && y >= 0 && y <= window_height) {
    color_buffer[(window_width * y) + x] = color;
  }
};

void render_color_buffer() {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer,
                    (int)(window_width * sizeof(color_t)));
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
};

void destroy_window(void) {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
};

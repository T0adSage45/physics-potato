#ifndef DISPLAY_H
#define DISPLAY_H
#include "vector.h"
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint32_t color_t;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *color_buffer_texture;
extern color_t *color_buffer; // declaration of first element of memory.
extern int window_width;
extern int window_height;
extern float grid_scale;

bool initalize_window(void);
void draw_pixel(int x, int y, color_t color);
void draw_grid(float grid_scale, color_t color);
void draw_line(int x, int y, int x1, int y1, color_t color);
void draw_rect(int x, int y, int width, int height, color_t color);
void render_color_buffer();
void clear_color_buffer(color_t color);
void destroy_window(void);

#endif

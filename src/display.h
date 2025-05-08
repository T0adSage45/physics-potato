#ifndef DISPLAY_H
#define DISPLAY_H
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *color_buffer_texture;
extern uint32_t *color_buffer; // declaration of first element of memory.
extern int window_width;
extern int window_height;
extern int grid_scale;

bool initalize_window(void);
void draw_pixel(int x, int y, uint32_t color);
void draw_grid();
void draw_line(int x, int y, int x1, int y1, uint32_t color);
void draw_rect(int x, int y, int width, int height, uint32_t color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                   uint32_t color);
void render_color_buffer();
void clear_color_buffer(uint32_t color);
void destroy_window(void);

#endif

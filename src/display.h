#ifndef DISPLAY_H
#define DISPLAY_H
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

enum render_method {
  RENDER_WIRE,
  RENDER_WIRE_VERTEX,
  RENDER_FILL_TRIANGLE,
  RENDER_FILL_TRIANGLE_WIRE
};

enum cull_method { CULL_NONE, CULL_BACKFACE };

typedef uint32_t color_t;

extern enum render_method render_method;
extern enum cull_method cull_method;
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
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                   color_t color);
void render_color_buffer();
void clear_color_buffer(color_t color);
void destroy_window(void);

#endif

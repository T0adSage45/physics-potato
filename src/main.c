#include "SDL2/SDL_platform.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include "wchar.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

bool is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool initalize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       800, 600, SDL_WINDOW_BORDERLESS);

  if (!window) {
    fprintf(stderr, "Error Creating SDL window.\n");
    return false;
  }
  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    fprintf(stderr, "Error Creating the SDL renderer. \n");
    return false;
  }
  return true;
}

int main(void) {
  is_running = initalize_window();
  return 0;
}

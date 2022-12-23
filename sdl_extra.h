#ifndef __SDL_EXTRA_H__
#define __SDL_EXTRA_H__

#include "status.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define WIN_TITLE "mandelbrot visualizer"

status_t SDL_setup(SDL_Window **win, SDL_Renderer **rend);
status_t SDL_cleanup(SDL_Window *win, SDL_Renderer *rend);

#endif


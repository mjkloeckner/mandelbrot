#include "sdl_extra.h"

status_t SDL_setup(SDL_Window **win, SDL_Renderer **rend) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        return ERROR_SDL_INIT;

    *win = SDL_CreateWindow(
		WIN_TITLE,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_RESIZABLE
    );

	*rend = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED);

	if ((*win == NULL) || (*rend == NULL))
		return ERROR_NULL_POINTER;

	if(TTF_Init() == -1)
		return ERROR_SDL_FONT_INIT;

	return OK;
}

status_t SDL_cleanup(SDL_Window *win, SDL_Renderer *rend) {
	if((win == NULL) || (rend == NULL))
		return ERROR_NULL_POINTER;

	TTF_Quit();
	SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
	SDL_Quit();
	return OK;
}

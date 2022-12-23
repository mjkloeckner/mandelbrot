#include "sdl_extra.h"
#include <stdbool.h>
#include <complex.h>

#define BG_COLOR	0x191919FF
#define MAX_IT		100

#define UNHEX(color) \
    ((color) >> (8 * 3)) & 0xFF, \
    ((color) >> (8 * 2)) & 0xFF, \
    ((color) >> (8 * 1)) & 0xFF, \
    ((color) >> (8 * 0)) & 0xFF

float map(float x, float in_min, float in_max, float out_min, float out_max);
void render_mandelbrot(int win_w, int win_h, SDL_Renderer *rend);

float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void render_mandelbrot(int win_w, int win_h, SDL_Renderer *rend) {
	size_t i, j, it;
	float complex z0, z;
	float real, imag, fg;
	for(i = 0; i < win_w; i++) {
		for(j = 0; j < win_w; j++) {
			/* z0 = map(i, 0, win_w, -2.00, 0.47) + map(j, 0, win_h, -1.12, 1.12) * I; */
			z0 = map(i, 0, win_w, -2.00, 1.00) + map(j, 0, win_h, -1.50, 1.50) * I;
			z = 0.0 + 0.0 * I;
			for(it = 0; (it <= MAX_IT) && (creal(z)*creal(z) + cimag(z)*cimag(z) <= 2*2); it++) {
				real = creal(z)*creal(z) - cimag(z)*cimag(z) + creal(z0);
				imag = 2*creal(z)*cimag(z) + cimag(z0);
				z = real + imag * I;
			}

			fg = map(it, 0, MAX_IT, 0, 1);
			uint8_t b = map(sqrt(fg), 0, 1, 0, 255);

			if(it == MAX_IT) b = 0;

			uint32_t color = (b << 24) + (b << 16) + (b << 8) + 0xFF;
			SDL_SetRenderDrawColor(rend, UNHEX(color));
			SDL_RenderDrawPoint(rend, i, j);
		}
	}
}

int main (int argc, char *argv[]) {
	SDL_Window *win;
	SDL_Renderer *rend;
	SDL_Event event;
	status_t st;
	bool run = true;
	int win_w, win_h;

	if((st = SDL_setup(&win, &rend)) != OK) {
		fprintf(stderr, "%s: couldn't initialize SDL2", argv[0]);
		return st;
	}

	win_w = 800;
	win_h = 600;
	while(run) {
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					run = false;
					break;
				case SDL_WINDOWEVENT:
					switch(event.window.event) {
					case SDL_WINDOWEVENT_RESIZED:
						win_w = event.window.data1;
						win_h = event.window.data2;
						break;
					default: break;
				}
				default: break;
			}
		}

		SDL_SetRenderDrawColor(rend, UNHEX(BG_COLOR));
		SDL_RenderClear(rend);

		render_mandelbrot(win_w, win_h, rend);

		SDL_RenderPresent(rend);
	}

	SDL_cleanup(win, rend);
	return 0;
}

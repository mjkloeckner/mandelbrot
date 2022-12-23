#include "sdl_extra.h"
#include <stdbool.h>
#include <complex.h>

#define BG_COLOR	0x191919FF
#define MAX_IT		100

#define DEFAULT_WIN_WIDTH 	1024
#define DEFAULT_WIN_HEIGHT 	720

#define WIDTH	640
#define HEIGHT	480

#define UNHEX(color) \
    ((color) >> (8 * 3)) & 0xFF, \
    ((color) >> (8 * 2)) & 0xFF, \
    ((color) >> (8 * 1)) & 0xFF, \
    ((color) >> (8 * 0)) & 0xFF

#define Re(z)	creal(z)
#define Im(z)	cimag(z)

float map(float x, float in_min, float in_max, float out_min, float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void render_mandelbrot(int win_w, int win_h, int width, int height, SDL_Renderer *rend) {
	size_t i, j, it;
	float complex z0, z;
	float real, imag, fg;
	for(i = 0; i < width; i++) {
		for(j = 0; j < height; j++) {
			z0 = map(i, 0, width, -5.50, 2.50) + map(j, 0, height, -2.00, 2.00) * I;
			z = 0.0 + 0.0 * I;
			for(it = 0; (it <= MAX_IT) && (Re(z)*Re(z) + Im(z)*Im(z) <= 4); it++) {
				real = Re(z)*Re(z) - Im(z)*Im(z) + Re(z0);
				imag = 2*Re(z)*Im(z) + Im(z0);
				z = real + imag * I;
			}

			fg = map(it, 0, MAX_IT, 0, 1);
			uint8_t b = map(sqrt(fg), 0, 1, 0, 255);

			if(it == MAX_IT) b = 0;

			uint32_t color = (b << 24) + (b << 16) + (b << 8) + 0xFF;
			SDL_SetRenderDrawColor(rend, UNHEX(color));
			SDL_RenderDrawPoint(rend, (win_w / 2) - (width / 2) + i, (win_h / 2) - (height / 2) + j);
		}
	}
}

int main (int argc, char *argv[]) {
	SDL_Window *win;
	SDL_Renderer *rend;
	SDL_Event event;

	status_t st;
	bool run = true;
	int win_w = DEFAULT_WIN_WIDTH, win_h = DEFAULT_WIN_HEIGHT;

	if((st = SDL_setup(&win, &rend)) != OK) {
		fprintf(stderr, "%s: couldn't initialize SDL2", argv[0]);
		return st;
	}

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

						SDL_SetRenderDrawColor(rend, UNHEX(BG_COLOR));
						SDL_RenderClear(rend);

						render_mandelbrot(win_w, win_h, 1600, 800, rend);

						SDL_RenderPresent(rend);
						break;
					default: break;
				}
				default: break;
			}
		}

	}

	SDL_cleanup(win, rend);
	return 0;
}

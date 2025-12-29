#include "mmenu.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "collide.h"
#include "base/lt.h"
#include "base/exitCodes.h"

bool quit;

// accessed in input.c
Entity *player;
double deltaTime;

void mMenu(void) {
	SDL_Texture *bg = textureLoad("res/main.bmp");

	SDL_Rect buttons[4] = {
		(SDL_Rect) { 115,900,375,115 },
		(SDL_Rect) { 550,900,375,115 },
		(SDL_Rect) { 985,900,375,115 },
		(SDL_Rect) { 1425,900,375,115 }
	};

	Uint64 nowT = SDL_GetPerformanceCounter();
	Uint64 lastT = 0;
	SDL_Event e;
	while(!quit) {
		lastT = nowT;
		nowT = SDL_GetPerformanceCounter();
		SDL_PollEvent(&e);
		switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONUP:{ // SDL_GetMouseState is evil!
				SDL_Rect rect = { e.button.x-10,e.button.y-10, 20,20 };

				for(int i = 0; i < 4; i++)
					if(DO_RECT_INTERSECT(&buttons[i], &rect))
						mMenuHandle(i);
			} break;
			default: break;
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_Rect rect = {0,0,1920,1080};
		SDL_RenderCopy(renderer, bg, NULL, &rect);

		SDL_RenderPresent(renderer);

		deltaTime = (double)((nowT-lastT)*10/(double)SDL_GetPerformanceFrequency());
	}
	cleanUp(E_SUCCESS);
}

void mMenuHandle(int opt) {
	switch (opt) {
		case 0: { // start

		} break;
		case 1: { // map

		} break;
		case 2: { // load

		} break;
		case 3: { // quit
			// TODO: are you sure?
			cleanUp(E_SUCCESS);
		} break;
	}
}

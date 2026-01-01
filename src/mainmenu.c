#include "mainmenu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <stdbool.h>
#include <string.h>

#include "collide.h"
#include "base/lt.h"
#include "base/exitCodes.h"
#include "graph.h"
#include "menu.h"
#include "scenes/ulrach1.h"

void mainMenu(void) {
	SDL_Texture *bg = textureLoad("res/main.bmp");

	Menu menu = {
		.items = malloc(sizeof(MenuItem)*5),
		.count = 5, 
		.id=0
	};
	memcpy(menu.items, (MenuItem[5]){
			(MenuItem) {
				.texture = bg,
				.pos=(SDL_Rect) { 0,0, width,height },
				.id=0
			},
			(MenuItem) {
				.pos=(SDL_Rect) { 115,900,375,115 },
				.cb=mainMenuHandle,
				.id=1
			},
			(MenuItem) {
				.pos=(SDL_Rect) { 550,900,375,115 },
				.cb=mainMenuHandle,
				.id=2
			},
			(MenuItem) {
				.pos=(SDL_Rect) { 985,900,375,115 },
				.cb=mainMenuHandle,
				.id=3
			},
			(MenuItem) {
				.pos=(SDL_Rect) { 1425,900,375,115 },
				.cb=mainMenuHandle,
				.id=4
			}
		}, sizeof(MenuItem)*5);
	menuSystemRegister(menu);
}

void mainMenuHandle(int _x, int opt) {
	switch (opt) {
		case 1: { // start
			menuSystemClear();
			ulrach();
		} break;
		case 2: { // map

		} break;
		case 3: { // load

		} break;
		case 4: { // quit
			// TODO: are you sure? menu
			cleanUp(E_SUCCESS);
		} break;
	}
}

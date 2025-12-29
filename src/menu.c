#include "menu.h"
#include "graph.h"
#include <SDL2/SDL_render.h>

void menuDraw(Menu menu) {
	for(int i = 0; i < menu.count; i++) {
		const MenuItem it = menu.items[i];
		if(!it.texture)
			SDL_RenderCopy(renderer, it.texture, NULL, &it.pos);
	}
}

void menuFreeTextures(Menu *menu) {
	for(int i = 0; i < menu->count; i++) {
		MenuItem *it  = &menu->items[i];
		SDL_DestroyTexture(it->texture);
		it->texture = NULL;
	}

}


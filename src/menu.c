#include "menu.h"
#include "base/alloc.h"
#include "base/arr.h"
#include "base/error.h"
#include "collide.h"
#include "graph.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

static Array_t _menus;

void menuDraw(Menu menu) {
	for(int i = 0; i < menu.count; i++) {
		const MenuItem it = menu.items[i];
		if(it.texture)
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

void menuSystemRegister(Menu menu) {
	if(!_menus.isValid) {
		UNWRAP_TO_COMPLEX_FN(
			array_init(pageAllocator(), Menu, 2*sizeof(Menu)), 
			_menus, Array_t);
	}
	array_push(&_menus, menu);
}
void menuSystemClear(void) {
	for(int i = 0; i < _menus.used; i++) {
		Menu menu;
		UNWRAP_TO(array_get(_menus, i), 
			menu, Menu);
		free(menu.items);
	}
	array_clear(&_menus);
}
void menuSystemDrawAll(void) {
	for(int i = 0; i < _menus.used; i++) {
		Menu menu;
		UNWRAP_TO(array_get(_menus, i), 
			menu, Menu);
		menuDraw(menu);
	}

}
void menuSystemInput(SDL_Event e) {
	if(e.type == SDL_MOUSEBUTTONUP) { // SDL_GetMouseState is evil!
		SDL_Rect rect = { e.button.x-10,e.button.y-10, 20,20 };

		for(int i = 0; i < _menus.used; i++) {
			Menu menu;
			UNWRAP_TO(array_get(_menus, i), 
				menu, Menu);
			for(int j = 0; j < menu.count; j++) {
				MenuItem it = menu.items[j];
				if(it.cb && DO_RECT_INTERSECT(&it.pos, &rect))
					it.cb(menu.id, it.id);
			}
		}
	} 
}


#ifndef _NTR_MENU_H_
#define _NTR_MENU_H_ 1
#include <SDL2/SDL.h>

typedef void (*menuitemcallback)(void);

typedef struct {
	SDL_Rect pos;
	SDL_Texture *texture;
	menuitemcallback cb;
} MenuItem;

typedef struct {
	MenuItem *items;
	int count;
} Menu;

void menuDraw(Menu menu);
void menuFreeTextures(Menu *menu);

#endif /* ifndef _NTR_MENU_H_ */


#include <SDL2/SDL_events.h>
#ifndef _NTR_MENU_H_
#define _NTR_MENU_H_ 1
#include <SDL2/SDL.h>

typedef void (*menuitemcallback)(int m_id, int mi_id);

typedef struct {
	SDL_Texture *texture;
	menuitemcallback cb;
	SDL_Rect pos;

	int id;
} MenuItem;

typedef struct {
	MenuItem *items;
	int count;

	int id;
} Menu;

void menuDraw(Menu menu);
void menuFreeTextures(Menu *menu);

/** Register a menu in the system, so
 * that the following functions can operate
 * on it. Initializes on the first call. */
void menuSystemRegister(Menu menu);
/** Remove all registered menus.
 * frees every menu item,
 * and destroys their textures */
void menuSystemClear(void);
/** Draw all registered menus with each of their
 * menu items if their texture is non-null */
void menuSystemDrawAll(void);
/** Handles mouse on-click events. Menu items 
 * in a 20x20px square around the cursor are 
 * called to handle the event */
void menuSystemInput(SDL_Event event);

#endif /* ifndef _NTR_MENU_H_ */


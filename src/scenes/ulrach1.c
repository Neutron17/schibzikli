#include "ulrach1.h"
#include "../tile.h"
#include "../graph.h"
#include "../movement.h"
#include "../moves.h"
#include <SDL2/SDL_render.h>

extern Entity *player;

void ulrach(void) {
	SDL_Texture *textures[] = {
		textureLoad("res/gras.bmp"),
		textureLoad("res/lava.bmp"),
		textureLoad("res/rock.bmp")
	};
	TileProp props[] = { 
		(TileProp){ 0.5, 0 }, 
		(TileProp) { 2, 10 }, 
		(TileProp) { 0.2, 0 } 
	};
	int tiles[] = { 
		2, 2, 2, 2, 2, 2, 2,
		2, 0, 1, 0, 0, 0, 2,
		2, 1, 1, 0, 1, 0, 2,
		2, 0, 1, 0, 0, 0, 2,
		2, 0, 0, 0, 0, 0, 2,
		2, 0, 0, 0, 0, 0, 2,
		2, 0, 0, 0, 0, 0, 2,
		2, 2, 2, 2, 2, 2, 2
	};
	TileEnv env = { 
		.textures=textures, 
		.properties=props, 
		.arena = arenaInit(TILESYS_SZ(3, 7, 8)).data,
		.len=3, 
		.w=7,
		.h=8 
	};
	tilemapLoad(env, tiles);

	struct EntityState st = { .health = 100, .speed = 20.0 };
	SDL_Texture *pltx = textureLoad("res/schibzi_atlas.bmp");

	player = ENTITY(POS(80,80), 200, 200, st, pltx);
	movementBindCb(player, movePlayer);
}

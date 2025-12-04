#include "tile.h"
#include "base/log.h"
#include "base/exitCodes.h"
#include "global.h"
#include "graph.h"
#include <SDL2/SDL_render.h>

static TileEnv env;

static int *tiles;

void tilemapDraw() {
	for(int i = 0; i < env.h; i++) {
		for(int j = 0; j < env.w; j++) {
     			const int index = tiles[i*env.w + j];
			if(index == -1)
				continue;
			SDL_Texture *tex = env.textures[index];
			SDL_Rect rect = { (float)width/2 - player->x + TILE_SZ*j, (float)height/2 - player->y + TILE_SZ*i, TILE_SZ, TILE_SZ};
			SDL_RenderCopy(renderer, tex, NULL, &rect);
		}
	}
}

void tilemapInit(TileEnv _env, const int *const _tiles) {
	tiles = malloc(sizeof(*tiles)*_env.w*_env.h);
	tilemapRefill(_env, _tiles);
}
void tilemapRefill(TileEnv _env, const int *const _tiles) {
	env = _env;
	memcpy(tiles, _tiles, sizeof(*tiles)*env.w*env.h);
}
void tilemapDestroy() {
	free(tiles);
}

TileProp tilePropFromPos(Pos pos) {
	if(pos.x > env.w || pos.y > env.h || pos.x < 0 || pos.y < 0)
		return (TileProp) { 1.0,0 };
	return env.properties[tiles[env.w*(pos.y/TILE_SZ) + pos.x/TILE_SZ]];;
}





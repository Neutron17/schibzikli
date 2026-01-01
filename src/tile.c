#include "tile.h"
#include "base/alloc.h"
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

void tilemapLoad(TileEnv _env, const int *const _tiles) {
	if(env.w*env.h == 0) 
		goto after;
	if(env.w*env.h < _env.w*_env.h) {
		arenaFree(&env.arena, tiles, sizeof(*tiles)*env.w*env.h);
		arenaFree(&env.arena, env.textures, sizeof(*env.textures)*env.len);
		arenaFree(&env.arena, env.properties, sizeof(*env.properties)*env.len);


		tiles = arenaAlloc(&_env.arena, sizeof(*_tiles)*_env.w*_env.h);
		env.textures = arenaAlloc(&_env.arena, sizeof(*_env.textures)*_env.len);
		env.properties = arenaAlloc(&_env.arena, sizeof(*_env.properties)*_env.len);
	}
after:
	env = _env;
	if(!tiles) {
		tiles = arenaAlloc(&env.arena, sizeof(*_tiles)*env.w*env.h);
		env.textures = arenaAlloc(&env.arena, sizeof(*env.textures)*env.len);
		env.properties = arenaAlloc(&env.arena, sizeof(*env.properties)*env.len);
	}
	memcpy(tiles, _tiles, sizeof(*_tiles)*env.w*env.h);
	memcpy(env.textures, _env.textures, sizeof(*_env.textures)*env.len);
	memcpy(env.properties, _env.properties, sizeof(*_env.properties)*env.len);
}
void tilemapDestroy() {
	free(tiles);
}

TileProp tilePropFromPos(Pos pos) {
	if(pos.x > env.w || pos.y > env.h || pos.x < 0 || pos.y < 0)
		return (TileProp) { 1.0,0 };
	return env.properties[tiles[env.w*(pos.y/TILE_SZ) + pos.x/TILE_SZ]];;
}





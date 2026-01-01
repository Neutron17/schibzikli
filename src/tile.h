/** -- tile.h --
 *  Tilesystem
 * The tilemap is made up of the identifiers of
 * each tile (int). The identifier -1 is not to 
 * be rendered. Identifiers less than -1 result
 * in undefined behaviour. 
 * The identifiers are not uniquely defined, they
 * correspond to the index of the 'textures' and 
 * 'properties' field in the 'TileEnv' struct.
 * 
 * */
#include "base/alloc.h"
#ifndef _NTR_TILE_H_
#define _NTR_TILE_H_ 1
#include <SDL2/SDL_render.h>
#include "graph.h"
#include "entitysystem.h"

// prefered size for arena
#define TILESYS_SZ(L,W,H) (sizeof(SDL_Texture *)*(L) + sizeof(TileProp)*(L) + sizeof(int)*(W)*(H))

struct TEntityNode {
	Entity entity;
	Entity *next;
};

typedef struct {
	struct TEntityNode *entityList;
	int type;
} Tile;

/** Tile Properties 
 * represent the attributes of a single tile */
typedef struct {
	/** 0 < speed_mul <= 1
	 * for default, set to 1 */
	double speed_mul;
	// for default, set to 0
	int dmg_p_sec;
} TileProp;

/** Tile Environment
 * Describes the environment in which tiles are interpreted.
 * corresponds to the whole system, NOT a single tile */
typedef struct {
	// 'textures' are freed by the tilesystem
	SDL_Texture **textures;
	// 'properties' are NOT freed by the tilesystem
	TileProp *properties;
	BareAllocator arena;
	int len;
	int w,h;
} TileEnv;

void tilemapLoad(TileEnv _env, const int *const _tiles);
void tilemapDestroy();

/** Copies the tile textures to the renderer
 * The tiles are defined with 'tilemapSet()'
 *
 * Note:
 *  'tilemapInit()' must be called beforehand */
void tilemapDraw();

struct EntityNode *tileEntitiesGet(int x, int y);
void tileEntityRegister(int x, int y);

TileProp tilePropFromPos(Pos pos);

#endif /* ifndef _NTR_TILE_H_ */


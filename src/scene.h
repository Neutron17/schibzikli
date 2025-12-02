#include <stdint.h>
#ifndef _NTR_SCENE_H_
#define _NTR_SCENE_H_ 1
#include "base/arr.h"
#include "entitysystem.h"

typedef struct {
	// of SDL_Texture
	Array_t textures;
	// of EntitySereal
	Array_t entities;
	// dinamically allocated
	uint32_t *tiles;
	uint32_t tile_w, tile_h;
} Scene;

Scene sceneParse(const char *fname, const char *resourcePath);
void sceneUnload(Scene *sc);

enum ParseType {
	// Begining of data
	PT_START 	= 0xFB,
	// End of data
	PT_END 		= 0xFE,

	PT_INSTR_B 	= 0x17,
	PT_INSTR_E 	= 0xFF
};

struct EntitySereal {
	float x,y;
	uint32_t w,h;
	uint8_t type;
	struct {
		uint32_t health;
		float thorn_percent;
		float shield_percent;
		float speed;
	} state;
	uint32_t texture;
};

// textures: Array of SDL_Texture
Entity entityserealToEntity(struct EntitySereal es, Array_t textures);

enum ParseInstruction {
/* 0x17 0x00 <path> NULL <path> NULL ...			0xFF */
	PI_TEXTURE = 0x00,
/* 0x17 0x01 <Entity> <Entity> ...				0xFF 
 *  Entity of type EntitySereal
 * No NULL bytes between elements
 * texture is the index to the texture array 
 * under PI_TEXTURE */
	PI_ENTITY,
/* 0x17 0x02 <4B_tileW> <4B_tileH> <tileW*tileH*4B_tileData>	(0xFF) */
	PI_TILE,
	PI_MAX
};
/*enum EntityType {
	ET_NO_STATE 	= 0,
	ET_HAS_HEALTH 	= 1 << 0,
	ET_ENEMY	= 1 << 1, // follows player
	ET_COLLIDE 	= 1 << 2,
	ET_STATIC 	= 1 << 3,
	ET_THORNS 	= 1 << 4,

	ET_DEFAULT = ET_HAS_HEALTH | ET_COLLIDE
};

struct EntityState {
	uint32_t health;
	float thorn_percent;
	float shield_percent;
	float speed;
};

typedef struct {
	float x,y;
	uint32_t w,h;
	enum EntityType type;
	struct EntityState state;
	SDL_Texture *texture;
	// 0 if entity is deleted
	uint32_t _id;
	// 0 if entity is not part of the movement system
	uint32_t _moveID;
} Entity;*/

#endif /* ifndef _NTR_SCENE_H_ */


/* -- entitysystem.h -- 
 * creates and destroys entities
 * all other tasks are delegated elsewhere
 * ex: entity damage is handled by dmg.h/.c
 * */
#ifndef _NTR_ENTITY_SYSTEM_H_
#define _NTR_ENTITY_SYSTEM_H_ 1
#include <SDL2/SDL.h>
#include "graph.h"

enum AnimState {
	AS_BACK,
	AS_BACK_1,
	AS_BACK_2,
	AS_FRONT,
	AS_FRONT_1,
	AS_FRONT_2,
	AS_SIDE_LEFT,
	AS_SIDE_LEFT_STEP,
	AS_SIDE_RIGHT,
	AS_SIDE_RIGHT_STEP,
};

enum EntityType {
	ET_NO_STATE 	= 0,
	ET_HAS_HEALTH 	= 1 << 0,
	ET_ENEMY	= 1 << 1, // follows player
	ET_COLLIDE 	= 1 << 2,
	ET_STATIC 	= 1 << 3,
	ET_THORNS 	= 1 << 4,

	ET_DEFAULT = ET_HAS_HEALTH | ET_COLLIDE
};

/** Extends some types in 'enum EntityType' with values */
struct EntityState {
	int health;
	float thorn_percent;
	float shield_percent;
	float speed;
	enum AnimState aState;
};

/**
 * Note: can be casted to 'PosF', and 'SDL_Rect' */
typedef struct {
	float x,y;
	int w,h;
	float vx,vy;
	enum EntityType type;
	struct EntityState state;
	SDL_Texture *texture;
	// 0 if entity is deleted
	int _id;
	// 0 if entity is not part of the movement system
	int _moveID;
} Entity;

void entitysystemInit(void);
void entitysystemDestroy(void);
Entity *entitysystemAdd(Entity *e);
/** Calls 'movementUnbind'
 * Destroys the texture */
void entitysystemRemove(Entity *e);
void entitysystemClear(void);
void entitysystemDrawAll(void);
Entity entitysystemGet(int id);
Entity *entitysystemGetPtr(int id);
Entity *_entity(Pos pos, int w, int h, enum EntityType type, struct EntityState state, SDL_Texture *texture);
#define ENTITY(P, W, H, S, T) _entity(P,W,H,ET_DEFAULT,S,T)

#endif // _NTR_ENTITY_SYSTEM_H_


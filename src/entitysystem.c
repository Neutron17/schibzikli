#include <SDL2/SDL_render.h>
#include <stdlib.h>
#include <pthread.h>

#include "entitysystem.h"
#include "base/alloc.h"
#include "base/arr.h"
#include "base/error.h"
#include "base/exitCodes.h"
#include "base/log.h"
#include "base/lt.h"
#include "graph.h"
#include "movement.h"
#include "global.h"

static Array_t entities;
static int id_counter = 1;
static pthread_mutex_t id_mutex;

void entitysystemInit(void) {
	UNWRAP_TO_COMPLEX_FN(
		array_init(arenaInitTyped(Entity, 8),Entity, 3), 
		entities, Array_t);
	if(pthread_mutex_init(&id_mutex, NULL)) {
		LOG(L_ERR, "Failed to create entity system mutex");
		cleanUp(E_ALLOC);
	}
}
void entitysystemDestroy(void) {
	array_destroy(&entities);
	pthread_mutex_destroy(&id_mutex);
}
void entitysystemClear(void) {
	array_clear(&entities);
}
Entity *entitysystemAdd(Entity *e) {
	pthread_mutex_lock(&id_mutex);
	e->_id = id_counter;
	id_counter++;
	pthread_mutex_unlock(&id_mutex);
	array_push(&entities, e);
	e = unwrap(array_lastptr(entities));
	return e;
}

Entity *_entity(Pos p, int w, int h, enum EntityType type, struct EntityState state, SDL_Texture *texture) {
	Entity ret = { p.x, p.y, w,h, 0.0,0.0,type, state,texture };
	return entitysystemAdd(&ret);
}

static int remover; // TODO: thread safety
static bool _remove_iter(const void *data) {
	Entity it = *(Entity *)data;
	if(it._id == remover)
		return true;
	return false;
}

void entitysystemRemove(Entity *e) {
	//remover = e->_id;
	if(e->_moveID)
		movementUnbind(*e);

	array_null(&entities, e->_id-1); // TODO: index shifts after remove

	SDL_DestroyTexture(e->texture);
	memset(e, 0, sizeof(*e));
}

static Pos spriteLUT[] = {
	[AS_BACK] = POS(64,2*64),
	[AS_BACK_1] = POS(0,0),
	[AS_BACK_2] = POS(0,64),
	[AS_FRONT] = POS(2*64, 64),
	[AS_FRONT_1] = POS(0,2*64),
	[AS_FRONT_2] = POS(0,3*64),
	[AS_SIDE_LEFT] = POS(2*64, 0),
	[AS_SIDE_LEFT_STEP] = POS(64,64),
	[AS_SIDE_RIGHT] = POS(64,3*64),
	[AS_SIDE_RIGHT_STEP] = POS(64,0),
};

void entitysystemDrawAll(void) {
	for(int i = 0; i < entities.used; i++) {
		Entity dest;
		UNWRAP_TO(array_index(entities, i), dest, Entity);
		if(dest._id == 0)
			continue;
		const SDL_Rect destRect = { 
			dest.x - player->x + (float)(width - dest.w)/2, 
			dest.y - player->y + (float)(height - dest.h)/2,
			dest.w, dest.h };
		if(dest._moveID == 0) {
			SDL_RenderCopy(renderer, dest.texture, NULL, &destRect);
		} else {
			const Pos p = spriteLUT[dest.state.aState];
			SDL_Rect srcrect = {
				p.x,p.y,
				64,64
			};
			SDL_RenderCopy(renderer, dest.texture, &srcrect, &destRect);

		}
		//printf("here %d\n", dest._id);
	}
}

Entity *entitysystemGetPtr(int id) {
	for(int i = 0; i < entities.used; i++) {
		Entity *ent;
		UNWRAP_TO_PTR(array_index(entities, i), ent, Entity);
		if(ent->_id == id)
			return ent;
	}
	return NULL;
}
Entity entitysystemGet(int id) {
	for(int i = 0; i < entities.used; i++) {
		Entity ent;
		UNWRAP_TO(array_index(entities, i), ent, Entity);
		if(ent._id == id)
			return ent;
	}
	return (Entity){0};
}


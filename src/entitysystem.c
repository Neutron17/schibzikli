#include <stdlib.h>
#include <pthread.h>

#include "entitysystem.h"
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
	UNWRAP_TO_COMPLEX(array_init(Entity, 3), entities, Array_t);
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
	Entity ret = { p.x, p.y, w,h, type, state,texture };
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

void entitysystemDrawAll(void) {
	for(int i = 0; i < entities.used; i++) {
		Entity dest;
		UNWRAP_TO(array_index(entities, i), dest, Entity);
		if(dest._id == 0)
			continue;
		/*const SDL_Rect playerRect = { 
			width/2 - (player->w/2), height/2 - (player->h/2), 
			player->w, player->h };*/
		const SDL_Rect entRect = { 
			dest.x - player->x + (float)(width - dest.w)/2, 
			dest.y - player->y + (float)(height - dest.h)/2,
			dest.w, dest.h };
		SDL_RenderCopy(renderer, dest.texture, NULL, &entRect);
		//printf("here %d\n", dest._id);
	}
}


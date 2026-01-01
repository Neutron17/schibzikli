#include "movement.h"
#include "base/alloc.h"
#include "base/arr.h"
#include "base/log.h"
#include "entitysystem.h"
#include "global.h"
#include "tile.h"
#include <SDL2/SDL_stdinc.h>

struct MovePair {
	Entity *e;
	movementCallbackFn cb;
};
// array of MovePairs
static Array_t movableEntityMap;
const static Uint8 *states;

void movementInit(void) {
	states = SDL_GetKeyboardState(NULL);
	UNWRAP_TO_COMPLEX_FN(
		array_init(arenaInitTyped(struct MovePair, 8), struct MovePair, 3), 
	     movableEntityMap, Array_t);
}
void movementDestroy(void) {
	array_destroy(&movableEntityMap);
}
/** Binds callback to entity, not thread safe */
void movementBindCb(Entity *entity, movementCallbackFn cb) {
	const struct MovePair pair = { entity, cb };
	array_push_ptr(&movableEntityMap, &pair);
	entity->_moveID = movableEntityMap.used;
	if(DEBUG)
		LOGF(L_DEBUG, "ADDED MOVE ID: %d\n", entity->_moveID);
}

void movementUnbind(Entity entity) {
	if(!entity._moveID)
		return;
	array_null(&movableEntityMap, entity._moveID-1);
}
static void movementForEach(void *data) {
	if(!data)
		return;
	struct MovePair pair = *(struct MovePair *)data;
	if(!pair.e)
		return;
	PosF *p = (PosF *)pair.e;
	const Pos pFloor = { floor(p->x), floor(p->y) };
	//printf("(%d %d)\n", p->x, p->y);
	*p = pair.cb(pair.e, states, tilePropFromPos(pFloor));
}
extern bool quit;
/** Calls the movement callback on each bound entity */
void movementMoveAll() {
	if(states[SDL_SCANCODE_ESCAPE] || states[SDL_SCANCODE_Q])
		quit = true;
	array_for_each(movableEntityMap, movementForEach);
}


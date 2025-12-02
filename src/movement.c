#include "movement.h"
#include "base/arr.h"
#include "base/log.h"
#include "entitysystem.h"
#include "global.h"
#include "tile.h"

struct MovePair {
	Entity *e;
	movementCallbackFn cb;
};
// array of MovePairs
static Array_t movableEntityMap;

void movementInit(void) {
	UNWRAP_TO_COMPLEX(array_init(struct MovePair, 3), movableEntityMap, Array_t);
}
void movementDestroy(void) {
	array_destroy(&movableEntityMap);
}
/** Binds callback to entity, not thread safe */
void movementBindCb(Entity *entity, movementCallbackFn cb) {
	const struct MovePair pair = { entity, cb };
	array_push(&movableEntityMap, &pair);
	entity->_moveID = movableEntityMap.used;
	if(DEBUG)
		LOGF(L_DEBUG, "ADDED MOVE ID: %d\n", entity->_moveID);
}

void movementUnbind(Entity entity) {
	if(!entity._moveID)
		return;
	array_null(&movableEntityMap, entity._moveID-1);
}
const static Uint8 *states;
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
	states = SDL_GetKeyboardState(NULL);
	if(states[SDL_SCANCODE_ESCAPE] || states[SDL_SCANCODE_Q])
		quit = true;
	array_for_each(movableEntityMap, movementForEach);
}


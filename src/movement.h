/** -- movement.h --
 * handles movement of entities
 * (player too)
 *
 * for callback implementations see moves.h
 * */
#ifndef _NTR_MOVE_H_
#define _NTR_MOVE_H_ 1
#include "entitysystem.h"
#include "graph.h"
#include "tile.h"
#include <SDL2/SDL_stdinc.h>

/** returns the new position */
typedef PosF (*movementCallbackFn)(Entity *self, const Uint8 *const keyboardState, TileProp tile);

void movementInit(void);
void movementDestroy(void);
/** Binds callback to entity, not thread safe */
void movementBindCb(Entity *entity, movementCallbackFn cb);
void movementUnbind(Entity entity);
/** Calls the movement callback on each bound entity */
void movementMoveAll();

#endif /* ifndef _NTR_MOVE_H_ */


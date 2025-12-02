#ifndef _NTR_COLLIDE_H_
#define _NTR_COLLIDE_H_ 1
#include "graph.h"
#include "entitysystem.h"
#include <stdbool.h>

#define DO_RECT_INTERSECT(A, B) SDL_HasIntersection((A), (B))
#define DO_CIRCLE_INTERSECT(a, b, a_r, b_e) ((POS_DIST(a, b) <= a_r+b_r) ? true : false)

// freed in graph.c
extern int *deltaMap;
extern Pos deltaDim;
void deltaMapInit(int w, int h, Pos *statics, int statics_len);

/** both 'a' and 'b' experience 
 * knockback  of equal magnitude 
 * (mass is not a property of the entitysystem) */
void collisionEllastic(Entity *restrict a, Entity *restrict b);
/** 'a' collides with 'b' but 'b' is super massive
 * so a will snap to edge of b */
void collisionStatic(Entity *a, Entity b);

#endif /* ifndef _NTR_COLLIDE_H_ */


#include "collide.h"

int *deltaMap;
Pos deltaDim;

void deltaMapInit(int w, int h, Pos *statics, int statics_len) {
	// w/2 * h/2 * sizeof(int) = (w*h*sizeof(int)) / 4 = w*h
	deltaMap = malloc(w*h); // freed in graph.c
	for(int i = 0; i < statics_len; i++) { // TODO: width
		statics[i].x <<= 1;
		statics[i].y <<= 1;
		statics[i].x >>= 1;
		statics[i].y >>= 1;
	}
}

/** both 'a' and 'b' experience 
 * knockback  of equal magnitude 
 * (mass is not a property of the entitysystem) */
void collisionEllastic(Entity *restrict a, Entity *restrict b) {

}
/** 'a' collides with 'b' but 'b' is super massive
 * so a will snap to edge of b */
void collisionSnap(Entity *a, Entity b) {
	a->x = b.x + ((float)b.w+a->w)/2;
	a->y = b.y + ((float)b.h+a->h)/2;
}


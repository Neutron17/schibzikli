#include "entitysystem.h"
#include "global.h"
#include "base/log.h"
#include "graph.h"
#include "moves.h"
#include "dmg.h"
#include "movement.h"

#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_stdinc.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

extern bool quit;
extern double deltaTime;
static Pos dest;

PosF movePlayer(Entity *self, const Uint8 *const keystates, TileProp tile) {
	PosF pl = *(PosF *)self;
	//printf("(%d %d) -> ", pl.x, pl.y);
	//const int speed = (self->state.speed + (keystates[SDL_SCANCODE_LSHIFT] ? self->state.speed/2 : 0)) * tile.speed_mul;
	
	dmgResolve(NULL, self, (Dmg){.type=DMG_TILE, .value=tile.dmg_p_sec});
	printf("%lf\n", tile.speed_mul);
	bool vflag = false, hflag = false;
	if(keystates[SDL_SCANCODE_A]) {
		player->vx = -self->state.speed*tile.speed_mul;
		hflag = true;
	}
	if(keystates[SDL_SCANCODE_D]) {
		player->vx = self->state.speed*tile.speed_mul;
		hflag = true;
	}
	if(keystates[SDL_SCANCODE_W]) {
		player->vy = -self->state.speed*tile.speed_mul;
		vflag = true;
	}
	if(keystates[SDL_SCANCODE_S]) {
		player->vy = self->state.speed*tile.speed_mul;
		vflag = true;
	}
	if(!vflag)
		player->vy = 0;
	if(!hflag)
		player->vx = 0;

	const PosLF v = POSLF(player->vx, player->vy);
	PosLF norm = POS_DIV(v, POS_MAGN(v));
	 //printf("Normal: (%lf, %lf)\n", norm.x, norm.y);
	static int counter = 0;
	counter++;

	int duration = 2000;

	if(norm.x > 0.7071) { // 1/sqrt(2)
		if(counter % (2*duration) == 0)
			player->state.aState = AS_SIDE_RIGHT;
		else if(counter % duration == 0)
			player->state.aState = AS_SIDE_RIGHT_STEP;
	} else if(norm.x < -0.7071) {
		if(counter % (2*duration) == 0)
			player->state.aState = AS_SIDE_LEFT;
		else if(counter % duration == 0)
			player->state.aState = AS_SIDE_LEFT_STEP;
	} else if(norm.y > 0.5) {
		if(counter % (2*duration) == 0)
			player->state.aState = AS_FRONT_1;
		else if(counter % duration == 0)
			player->state.aState = AS_FRONT_2;
	} else if(norm.y < -0.5) {
		if(counter % (2*duration) == 0)
			player->state.aState = AS_BACK_1;
		else if(counter % duration == 0)
			player->state.aState = AS_BACK_2;
	} else { // stationary
		if(counter % 20 == 0) {
			switch (player->state.aState) {
				case AS_SIDE_RIGHT:
				case AS_SIDE_RIGHT_STEP:
					player->state.aState = AS_SIDE_RIGHT;
					break;
				case AS_SIDE_LEFT:
				case AS_SIDE_LEFT_STEP:
					player->state.aState = AS_SIDE_LEFT;
					break;
				case AS_BACK:
				case AS_BACK_1:
				case AS_BACK_2:
					player->state.aState = AS_BACK;
					break;
				case AS_FRONT:
				case AS_FRONT_1:
				case AS_FRONT_2:
					player->state.aState = AS_FRONT;
					break;
				default:;
			}
		}
	}
	const PosF delta = POSF(
		player->vx * deltaTime,
		player->vy * deltaTime
		//speed * (keystates[SDL_SCANCODE_D]-keystates[SDL_SCANCODE_A]),
		//speed * (keystates[SDL_SCANCODE_S]-keystates[SDL_SCANCODE_W])
	);
	POS_ADD2(pl, delta);
	if(DEBUG)
		LOGF(L_DEBUG, "Player moved to (%f %f)", pl.x, pl.y);
	return pl;
}

PosF moveGimp(Entity *self, const Uint8 *const keystates, TileProp tile) {
	if(POS_DIST(*(PosF *)self, *(PosF *)player) < 0.5)
		return *(PosF *)self;
	const double a = atan2(player->y - self->y, player->x - self->x);
	const double speeeed = self->state.speed*tile.speed_mul;
	//Pos p = POS(abs(player.x-self->x), 2.0/abs(player.y-self->y));
	const PosF delta = { speeeed*cos(a), speeeed*sin(a) };
	const PosF p = POSF_ADD(*(PosF *)self, delta);
	//PosF p = POSF(self->x + speeeed*cos(a), self->y + speeeed*sin(a));
	dmgResolve(NULL, self, (Dmg){DMG_TILE, tile.dmg_p_sec});
	if(DEBUG)
		LOGF(L_DEBUG, "Gimp moved to (%f %f)", p.x, p.y);
	return p;
}

/*
void mouseFollow(Pos dest) {
	if(abs(dest.x - player->x) > 4 && abs(round(dest.y - player->y)) > 4) {
		const int dx = dest.x - player.x;
		const int dy = dest.y - player.y;
		const double magn = sqrt(dx*dx + dy*dy); // Pythagoras
		const double vx = dx / magn;
		const double vy = dy / magn;

		player.x += vx * SPEED;
		player.y += vy * SPEED;
	}
}
**/

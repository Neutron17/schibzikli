#include <SDL2/SDL_render.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <SDL2/SDL.h>

#include "base/arr.h"
#include "base/exitCodes.h"
#include "base/log.h"
#include "base/lt.h"

#include "global.h"
#include "graph.h"
#include "entitysystem.h"
#include "movement.h"
#include "moves.h"
#include "tile.h"
#include "dmg.h"
#include "scene.h"

#define DELAY (1000/60)

// accessed in input.c
bool quit;
static Uint64 deltaTime;
Entity *player;

void sighandl(int signal);

int main(int argc, char *argv[]) {
	init();
	signal(SIGINT, sighandl);
	/*Scene sc = sceneParse("data.dat", ".");
	array_print(sc.textures);
	array_print(sc.entities);
	printf("%d %d %d\n", ((struct EntitySereal *)sc.entities.members)->w,
((struct EntitySereal *)sc.entities.members)->h,
((struct EntitySereal *)sc.entities.members)->type
	);
	array_destroy(&sc.textures);
	array_destroy(&sc.entities);
	return 0;*/

	SDL_Texture *playerTexture = textureLoad("box.bmp");
	SDL_Texture *gimpTex = textureLoad("gimp.bmp");
	SDL_Texture *rockTex = textureLoad("rock.bmp");

	SDL_Texture *tileTexture = textureLoad("1.bmp");
	SDL_Texture *lava = textureLoad("lava.bmp");

	//player = entity(width/2, height/2, 50, 50, playerTexture);
	const struct EntityState st = (struct EntityState){ .health = 100, .speed = 15.0 };
	player = ENTITY(POS(80, 80), 50, 50, st, playerTexture);
	movementBindCb(player, movePlayer);

	Entity *gimp = _entity(POS(50,50), 75, 75, 
			ET_HAS_HEALTH | ET_ENEMY | ET_COLLIDE, 
			(struct EntityState) { .health=200, .speed = 2.0 }, 
			gimpTex);
	movementBindCb(gimp, moveGimp);

	Entity *rock = _entity(POS(50, 50), 70, 70, 
			ET_STATIC | ET_COLLIDE | ET_NO_STATE, 
			(struct EntityState){0}, 
			rockTex);
	
	Uint64 startT;
	SDL_Event e;
	SDL_Texture *textures[] = { tileTexture, lava, rockTex };
	TileProp props[] = { 
		(TileProp){ 0.5, 0 }, 
		(TileProp) { 2, 10 }, 
		(TileProp) { 0, 0 } 
	};
	TileEnv env = { .textures=textures, .properties=props, .len=3 };
	int tiles[] = { 
		2, 2, 2, 2, 2, 2, 2,
		2, 0, 1, 0, 0, 0, 2,
		2, 1, 1, 0, 1, 0, 2,
		2, 0, 1, 0, 0, 0, 2,
		2, 0, 0, 0, 0, 0, 2,
		2, 0, 0, 0, 0, 0, 2,
		2, 0, 0, 0, 0, 0, 2,
		2, 2, 2, 2, 2, 2, 2
	};
	tilemapSet(env, tiles, 7, 8);
	while(!quit) {
		startT = SDL_GetTicks();
		SDL_PollEvent(&e);
		switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_WINDOWEVENT:
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					SDL_GetWindowSize(window, &width, &height);
				break;
			default: break;
		}

		movementMoveAll();
		//printf("%d %d\n", player.x, player.y);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		tilemapDraw();
		entitysystemDrawAll();

		SDL_RenderPresent(renderer);

		deltaTime = SDL_GetTicks64() - startT;
		if(DELAY > deltaTime)
			SDL_Delay(DELAY - deltaTime);
	}
	entitysystemRemove(player);
	entitysystemRemove(rock);
	SDL_DestroyTexture(tileTexture);
	SDL_DestroyTexture(lava);
	cleanUp(E_SUCCESS);
}

void init(void) {
	loginit(L_DEBUG, L_DEBUG);
	graphInit("Title", 500, 500);
	entitysystemInit();
	movementInit();
}
_Noreturn void cleanUp(int status) {
	movementDestroy();
	entitysystemDestroy();
	logdestroy();
	graphDestroy();
	exit(status);
}

void sighandl(int signal) {
	if(signal == SIGINT)
		cleanUp(E_SIGNAL);
}


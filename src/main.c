#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
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

// accessed in input.c
bool quit;
double deltaTime;
Entity *player;

void sighandl(int signal);

int main(int argc, char *argv[]) {
	init();
	signal(SIGINT, sighandl);

	SDL_Texture *playerTexture = textureLoad("res/schibzi_atlas.bmp");
	const struct EntityState st = {
		.health = 100,
		.speed = 30.0
	};
	player = ENTITY(POS(80,80), 100, 100, st, playerTexture);
	movementBindCb(player, movePlayer);


	SDL_Texture *grasTex = textureLoad("res/gras.bmp");
	TileProp props[1] = { 
		(TileProp) {
			1.0, 0
		}
	};
	int tiles[] = {
		0,0,0, 0,
		0,0,0, 0,
		0,0,0, 0,
		0,0,0, 0
	};
	tilemapInit((TileEnv) { (SDL_Texture *[]){grasTex}, props, 1, 4, 4, NULL }, tiles);

	Uint64 nowT = SDL_GetPerformanceCounter();
	Uint64 lastT = 0;
	SDL_Event e;
	while(!quit) {
		lastT = nowT;
		nowT = SDL_GetPerformanceCounter();
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
		////printf("%d %d\n", player->x, player->y);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		tilemapDraw();
		entitysystemDrawAll();

		SDL_RenderPresent(renderer);

		deltaTime = (double)((nowT-lastT)*10/(double)SDL_GetPerformanceFrequency());

		//if(DELAY > deltaTime)
			//SDL_Delay(DELAY - deltaTime);
	}
	entitysystemRemove(player);
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


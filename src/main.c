#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "base/alloc.h"
#include "base/arr.h"
#include "base/error.h"
#include "base/exitCodes.h"
#include "base/log.h"
#include "base/lt.h"

#include "global.h"
#include "graph.h"
#include "entitysystem.h"
#include "mainmenu.h"
#include "movement.h"
#include "dmg.h"
#include "scene.h"
#include "menu.h"

void sighandl(int signal);
void gameLoop(void);

bool quit;
// accessed in input.c
Entity *player;
double deltaTime;

int main(int argc, char *argv[]) {
	init();
	mainMenu();
	gameLoop();
	cleanUp(E_SUCCESS);
}

void gameLoop() {
	Uint64 nowT = SDL_GetPerformanceCounter();
	Uint64 lastT = 0;
	SDL_Event e;
	while(!quit) {
		lastT = nowT;
		nowT = SDL_GetPerformanceCounter();
		SDL_PollEvent(&e);
		menuSystemInput(e);
		switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;
			default: break;
		}
		movementMoveAll();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		tilemapDraw();
		entitysystemDrawAll();
		menuSystemDrawAll();

		SDL_RenderPresent(renderer);

		deltaTime = (double)((nowT-lastT)*10/(double)SDL_GetPerformanceFrequency());
	}
}

void init(void) {
	loginit(L_DEBUG, L_DEBUG);
	graphInit("Schibzikli", 500, 500);
	TTF_Init();
	entitysystemInit();
	movementInit();
	//signal(SIGINT, sighandl);
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


#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "base/exitCodes.h"
#include "base/log.h"
#include "base/lt.h"

#include "global.h"
#include "graph.h"
#include "entitysystem.h"
#include "mmenu.h"
#include "movement.h"
#include "dmg.h"
#include "scene.h"

void sighandl(int signal);

int main(int argc, char *argv[]) {
	init();

	mMenu();
}

void init(void) {
	signal(SIGINT, sighandl);
	loginit(L_DEBUG, L_DEBUG);
	graphInit("Schibzikli", 500, 500);
	TTF_Init();
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


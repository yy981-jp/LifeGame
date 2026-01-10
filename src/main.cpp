#define SDL_MAIN_HANDLED

#include <thread>

#include <SDL2/SDL.h>

#include <yy981/time.h>

#include "Game.h"


Game* game;

namespace param {
	constexpr unsigned int 
		fpsDelayMS = 1000 / 60; // 60fps
}

void eventloop() {
	SDL_Event event;
	bool quit = false;
	while (!quit) {
		game->update();
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) quit = true;
		}
		sleepc(tu::l, param::fpsDelayMS);
	}
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	game = new Game;

	eventloop();

	delete game;
	SDL_Quit();
}

#define SDL_MAIN_HANDLED

#include <thread>
#include <string>
#include <stdexcept>

#include <SDL2/SDL.h>

#include <yy981/time.h>
#include <yy981/string.h>

#include "Game.h"


Game* game;

namespace param {
	int FPS = 60,
		fpsDelayMS = 1000 / FPS;
}

struct carg {
	int windowWidth = 860,
		windowHeight = 600,
		scale = 10,			// scale:1 = window*:cellIdxSize_*
		prob = 4;
};

void eventloop() {
	SDL_Event event;
	bool quit = false;
	while (!quit) {
		game->update();
		game->updateCursor();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_MOUSEBUTTONDOWN:
				game->onMouseButton(event.button);
				break;

			case SDL_KEYDOWN:
				game->onKey(event.key);
				break;

			case SDL_KEYUP:
				game->onKeyUP(event.key);
				break;
			
			case SDL_MOUSEWHEEL:
				game->onMouseWheel(event.wheel);
				break;
			case SDL_MOUSEMOTION:
				game->onMouseMotion(event.motion);
				break;
			}
		}
		sleepc(tu::l, param::fpsDelayMS);
	}
}

carg parseCLIArg(const std::vector<std::string>& arg) {
	carg result;
	bool l1 = true;
	for (const std::string& p: arg) {
		if (l1) { // argv[0]を飛ばす
			l1 = false;
			continue;
		}
		if (!p.starts_with("-")) throw std::runtime_error("引数エラー [-]");
		switch (p[1]) {
			case 'w': result.windowWidth = std::stoi(p.substr(2)); break;
			case 'h': result.windowHeight = std::stoi(p.substr(2)); break;
			case 's': result.scale = std::stoi(p.substr(2)); break;
			case 'r': result.prob = std::stoi(p.substr(2)); break;
			case 'f': { 
				param::FPS = std::stoi(p.substr(2));
				param::fpsDelayMS = 1000 / param::FPS;
			} break;
			default: throw std::runtime_error("引数エラー [" + std::string{p[1]} + "]");
		}
	}
	if (result.scale < 1) throw std::runtime_error("scale < 1");
	if (result.prob <= 0) throw std::runtime_error("prob <= 0");
/*	if (result.windowHeight % result.scale || result.windowWidth % result.scale)
		throw std::runtime_error("result.windowHeight % result.scale == 0 or result.windowWidth % result.scale == 0");*/
	return result;
}

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());

	carg prm = parseCLIArg(st::charV(argc,argv));
	game = new Game(prm.windowWidth,prm.windowHeight,prm.scale,prm.prob);

	eventloop();

	delete game;
	SDL_Quit();
}

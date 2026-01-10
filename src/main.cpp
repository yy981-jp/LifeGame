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
	int fpsDelayMS = 1000 / 60; // 60fps
}

struct carg {
	int windowWidth = 860,
		windowHeight = 600,
		scale = 10;			// scale:1 = window*:cellIdxSize_*
};

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
			case 'f': param::fpsDelayMS = 1000 / std::stoi(p.substr(2)); break;
			default: throw std::runtime_error("引数エラー [" + std::string{p[1]} + "]");
		}
	}
	if (result.scale < 1) throw std::runtime_error("scale < 1");
/*	if (result.windowHeight % result.scale || result.windowWidth % result.scale)
		throw std::runtime_error("result.windowHeight % result.scale == 0 or result.windowWidth % result.scale == 0");*/
	return result;
}

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());

	carg prm = parseCLIArg(st::charV(argc,argv));
	game = new Game(prm.windowWidth,prm.windowHeight,prm.scale);

	eventloop();

	delete game;
	SDL_Quit();
}

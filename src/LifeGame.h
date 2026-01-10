#pragma once
#include <vector>

#include <yy981/random.h>

namespace param {
	extern int FPS, fpsDelayMS;
}



class LifeGame {
    std::vector<uint8_t> cells;

public: 
    int width, height, prob;
    bool running = true;
    size_t gen = 0;

    LifeGame(int width = 80, int height = 60, int prob = 4):
      width(width), height(height), prob(prob), cells(width*height,0) {
        fillRandom();
    }

    int flater(int x, int y) {
        return y * width + x;
    }

    const std::vector<uint8_t>& data() const {
        return cells;
    }

    void clear() {
        cells.clear();
        cells.resize(width*height);
    }

    void step();

    void toggle(int x, int y, bool value) {
        cells[flater(x,y)] = value;
    }

    void fillRandom() {
        for (int y = 1; y < height-1; ++y)
            for (int x = 1; x < width-1; ++x)
                cells[flater(x,y)] = !randomNum(0, prob-1);
    }

    bool scrollFPS(bool dire) { // true -> up, false -> down
        int nextFPS = -1;
        if (dire) nextFPS = param::FPS + 1;
            else nextFPS = param::FPS - 1;
        if (nextFPS < 1 || nextFPS > 1001) return false;
        param::FPS = nextFPS;
        param::fpsDelayMS = 1000 / nextFPS;
        return true;
    }
};

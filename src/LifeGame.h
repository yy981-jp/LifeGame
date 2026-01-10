#pragma once
#include <vector>

#include <yy981/random.h>


class LifeGame {
    std::vector<uint8_t> cells;
    int width, height;

public: 
    LifeGame(int width = 80, int height = 60):
      width(width), height(height), cells(width*height,0) {
        for (int y = 1; y < height-1; ++y)
            for (int x = 1; x < width-1; ++x)
                cells[flater(x,y)] = (randomNum(0, 3) == 0); // 25%
    }

    int flater(int x, int y) {
        return y * width + x;
    }

    const std::vector<uint8_t>& data() const {
        return cells;
    }

    void step();
};

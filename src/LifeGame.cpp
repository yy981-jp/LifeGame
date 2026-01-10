#include "LifeGame.h"


    void LifeGame::step() {
        std::vector<uint8_t> nextCells(width*height,0);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = flater(x,y);
                if (x==0 || x==width-1 || y==0 || y==height-1) {
                    nextCells[index] = false; // マップ端は無条件に死滅
                    continue;
                }
                if (cells[index]) {
                    // 生きている場合
                    uint8_t count = 0;
                    for (int cy = -1; cy <= 1; ++cy)
                        for (int cx = -1; cx <= 1; ++cx)
                            if ((cx || cy) && cells[flater(x+cx,y+cy)]) ++count;
                    if (count <= 1 || count >= 4) nextCells[index] = false; // 死滅: 過疎(<=1), 過密(>=4) , else 生存(cell変化なし)
                        else nextCells[index] = true;
                } else {
                    // 死んでいる場合
                    uint8_t count = 0;
                    for (int cy = -1; cy <= 1; ++cy)
                        for (int cx = -1; cx <= 1; ++cx)
                            if (cells[flater(x+cx,y+cy)]) ++count;
                    if (count == 3) nextCells[index] = true;
                }
            }
        }
        cells = std::move(nextCells);
    }

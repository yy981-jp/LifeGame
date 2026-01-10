#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include "LifeGame.h" 

class Game {
    int windowWidth,
		windowHeight,
        cellIdxSize_y, // y方向のcell数
        cellIdxSize_x; // x方向のcell数
    
    uint32_t ARGB_white = 0xFFFFFFFF,
             ARGB_black = 0xFF000000;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    LifeGame life;


public:
    Game(int windowWidth = 800, int windowHeight = 600, int scale = 10);
    ~Game();
    void update();
};

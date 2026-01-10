#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <string>

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


    void updateWindowTitle() {
        std::string text = 
            "LifeGame: " + std::to_string(param::FPS) + "FPS" + ((param::FPS>1000)? "(無限)": "") + ", " 
            + std::to_string(life.gen) + "世代 " 
            + (life.running? "実行中": "停止中");
        SDL_SetWindowTitle(window,text.c_str());
    }

    void scrollFPS(bool dire) {
        if (life.scrollFPS(dire)) updateWindowTitle();
    }


public:
    Game(int windowWidth = 800, int windowHeight = 600, int scale = 10, int prob = 4);
    ~Game();
    void update();

    void onMouseButton(const SDL_MouseButtonEvent e) {
    	if (e.button == SDL_BUTTON_LEFT) life.toggle(e.x, e.y, true);
        else if (e.button == SDL_BUTTON_RIGHT) life.toggle(e.x, e.y, false);
    }

    void onKey(const SDL_KeyboardEvent e) {
        switch (e.keysym.sym) {
            case SDLK_SPACE: {
                if (!e.repeat) {
                    life.running = !life.running;
                    updateWindowTitle();
                }
            } break;
            case SDLK_c: {
                life.clear();
            } break;
            case SDLK_r: life.fillRandom(); break;
            case SDLK_UP: scrollFPS(true); break;
            case SDLK_DOWN: scrollFPS(false); break;
            case SDLK_t: updateWindowTitle(); break;
            default: return;
        }
    }

    void onMouseWheel(const SDL_MouseWheelEvent e) {
        if (e.y >= 1) scrollFPS(true);
            else scrollFPS(false);
    }


};

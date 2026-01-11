#pragma once
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <string>

#include "def.h"
#include "LifeGame.h"
#include "fsutil.h"

class Game {
    int windowWidth,
		windowHeight,
        cellIdxSize_y, // y方向のcell数
        cellIdxSize_x, // x方向のcell数
        mouseCursorPos_x,
        mouseCursorPos_y,
        mouseMove_x,
        mouseMove_y,
        structureWidth,
        structureHeight;
    uint32_t ARGB_white = 0xFFFFFFFF,
             ARGB_black = 0xFF000000;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    std::vector<std::vector<uint8_t>> structure;
    bool settingStructure = false;
    SDL_Texture* structureTexture = nullptr;

    LifeGame life;

    bool key_md_shift = false,
         key_md_ctrl = false;


    void updateWindowTitle() {
        std::string text = 
            "LifeGame: " + std::to_string(param::FPS) + "FPS" + ((param::FPS>1000)? "(無限)": "") + ", " 
            + std::to_string(life.gen) + "世代 " 
            + (life.running? "実行中": "停止中");
        SDL_SetWindowTitle(window,text.c_str());
    }

    void scrollFPS(bool dire) {
        if (life.scrollFPS(dire,key_md_shift)) updateWindowTitle();
    }

public:
    Game(int windowWidth = 800, int windowHeight = 600, int scale = 10, int prob = 4);
    ~Game();

    void update();

    void updateCursor() {
        if (!mouseMove_x && !mouseMove_y) return;
        int x, y;
        SDL_GetMouseState(&x,&y);
        SDL_WarpMouseInWindow(window, x+mouseMove_x, y+mouseMove_y);
    }


    void onMouseButton(const SDL_MouseButtonEvent& e) {
    	if (!settingStructure) {
            if (e.button == SDL_BUTTON_LEFT) life.toggle(e.x, e.y, true);
            else if (e.button == SDL_BUTTON_RIGHT) life.toggle(e.x, e.y, false);
        } else {
            for (int y = 0; y < structureHeight; ++y) {
                for (int x = 0; x < structureWidth; ++x) {
                    life.toggle(mouseCursorPos_x+x, mouseCursorPos_y+y,structure[y][x]);
                }
            }
            settingStructure = false;
        }
    }

    void onKey(const SDL_KeyboardEvent& e);
    void onKeyUP(const SDL_KeyboardEvent& e);

    void onMouseWheel(const SDL_MouseWheelEvent& e) {
        if (e.y >= 1) scrollFPS(true);
            else scrollFPS(false);
    }

    void onMouseMotion(const SDL_MouseMotionEvent& e) {
        mouseCursorPos_x = e.x;
        mouseCursorPos_y = e.y;
    }

    json generateJson();
    void parseJson(const json& j);

    void openFile() {
        std::string path = openFileDialog();
        if (path.empty()) return;
        if (path.ends_with(".json")) loadJson(path);
        else if (path.ends_with(".lgs")) loadLGS(path);
        else throw std::runtime_error("Game::openFile(): not json lgs");
    }

    void saveJson() {
        json j = generateJson();
        std::string path = saveFileDialog();
        if (path.empty()) return;
        writeJsonFile(j,path);
    }

    void loadJson(std::string path) {
        json j = readJsonFile(path);
        parseJson(j);
    }

    void configureLGStructure();

    void loadLGS(std::string path) {
        structure = std::move(readLGSFile(path));
        configureLGStructure();
    }

};

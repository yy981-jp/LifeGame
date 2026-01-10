#include <string>
#include <stdexcept>

#include "Game.h"

    Game::Game(int windowWidth, int windowHeight, int cellIdxSize_x, int cellIdxSize_y):
      windowWidth(windowWidth), windowHeight(windowHeight), cellIdxSize_x(cellIdxSize_x), cellIdxSize_y(cellIdxSize_y), life(cellIdxSize_x,cellIdxSize_y) {
        window = SDL_CreateWindow(
            "LifeGame",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            windowWidth,
            windowHeight,
            SDL_WINDOW_SHOWN
        );
        if (!window) throw std::runtime_error(std::string("SDL_CreateWindow failed: %s\n") + SDL_GetError());

        renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
        );
        if (!renderer) throw std::runtime_error(std::string("SDL_CreateRenderer failed: %s\n") + SDL_GetError());


        texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            cellIdxSize_x,
            cellIdxSize_y
        );
        if (!texture) throw std::runtime_error(std::string("SDL_CreateTexture failed: %s\n") + SDL_GetError());


        // 論理サイズ
        SDL_RenderSetLogicalSize(renderer, cellIdxSize_x, cellIdxSize_y);

        // 背景クリア（黒）
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    Game::~Game() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(texture);
    }

    void Game::update() {
        uint32_t* pixels;
        int pitch;
        SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch);

        life.step();
        const std::vector<uint8_t>& cells = life.data();

        for (int y = 0; y < cellIdxSize_y; ++y) {
            for (int x = 0; x < cellIdxSize_x; ++x) {
                int index = life.flater(x,y);
                pixels[index] = (cells[index]? ARGB_white: ARGB_black);
            }
        }

        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

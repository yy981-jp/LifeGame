#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>


class Game {
    int windowWidth = 800,
		windowHeight = 600,
		cellSize = 10,
        cellIdxSize_y = -1, // y方向のcell数
        cellIdxSize_x = -1; // x方向のcell数
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

public: 
    Game(int windowWidth = 800, int windowHeight = 600, int cellSize = 10):
      windowWidth(windowWidth), windowHeight(windowHeight), cellSize(cellSize) {
        
        cellIdxSize_x = windowWidth / cellSize;
        cellIdxSize_y = windowHeight / cellSize;
        
        window = SDL_CreateWindow(
            "LifeGame",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            windowWidth,
            windowHeight,
            SDL_WINDOW_SHOWN
        );

        renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
        );

        texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            cellIdxSize_x,
            cellIdxSize_y
        );

        // 論理サイズ
        SDL_RenderSetLogicalSize(renderer, cellIdxSize_x, cellIdxSize_y);

        // 背景クリア（黒）
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    ~Game() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_DestroyTexture(texture);
    }

    void update() {
        uint32_t* pixels;
        int pitch;
        SDL_LockTexture(texture, nullptr, (void**)&pixels, &pitch);
        int stride = pitch / sizeof(uint32_t);

        for (int y = 0; y < cellIdxSize_y; ++y) {
            for (int x = 0; x < cellIdxSize_x; ++x) {
                pixels[y*stride + x] = 0xFF0000FF; // 青
            }
        }

        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }
};
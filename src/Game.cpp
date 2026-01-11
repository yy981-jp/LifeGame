#include <string>
#include <stdexcept>

#include "Game.h"

    Game::Game(int windowWidth, int windowHeight, int scale, int prob):
      windowWidth(windowWidth), windowHeight(windowHeight), cellIdxSize_x(windowWidth/scale), cellIdxSize_y(windowHeight/scale),
      life(cellIdxSize_x,cellIdxSize_y,prob) {
        window = SDL_CreateWindow(
            "LifeGame",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            windowWidth,
            windowHeight,
            SDL_WINDOW_SHOWN
        );
        if (!window) throw std::runtime_error(std::string("SDL_CreateWindow failed: ") + SDL_GetError());

        renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED
        );
        if (!renderer) throw std::runtime_error(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());


        texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            cellIdxSize_x,
            cellIdxSize_y
        );
        if (!texture) throw std::runtime_error(std::string("SDL_CreateTexture failed: ") + SDL_GetError());


        // 論理サイズ
        SDL_RenderSetLogicalSize(renderer, cellIdxSize_x, cellIdxSize_y);

        // 背景クリア（黒）
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // カーソル描画用
        SDL_ShowCursor(SDL_DISABLE);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 160, 255, 127);

        updateWindowTitle();
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

        if (life.running) life.step();
        const std::vector<uint8_t>& cells = life.data();

        for (int y = 0; y < cellIdxSize_y; ++y) {
            for (int x = 0; x < cellIdxSize_x; ++x) {
                int index = life.flater(x,y);
                pixels[index] = (cells[index]? ARGB_white: ARGB_black);
            }
        }

        SDL_UnlockTexture(texture);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);

        if (mouseCursorPos_x >= 0 && mouseCursorPos_x < life.width
          && mouseCursorPos_y >= 0 && mouseCursorPos_y < life.height) {
            SDL_RenderDrawPoint(renderer, mouseCursorPos_x, mouseCursorPos_y);
        }

        SDL_RenderPresent(renderer);
    }

    void Game::onKey(const SDL_KeyboardEvent& e) {
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
            case SDLK_t: updateWindowTitle(); break;
            case SDLK_LSHIFT: key_md_shift = true; break;
            case SDLK_LCTRL: key_md_ctrl = true; break;
            case SDLK_RETURN: life.toggle(mouseCursorPos_x, mouseCursorPos_y); break;

            case SDLK_UP: {
                if (key_md_ctrl||key_md_shift) scrollFPS(true);
                    else mouseMove_y = -1;
            } break;
            case SDLK_DOWN: {
                if (key_md_ctrl||key_md_shift) scrollFPS(false);
                    else mouseMove_y = 1;
            } break;
            case SDLK_LEFT: mouseMove_x = -1; break;
            case SDLK_RIGHT: mouseMove_x = 1; break;
            default: return;
        }
    }

    void Game::onKeyUP(const SDL_KeyboardEvent& e) {
        switch (e.keysym.sym) {
            case SDLK_LSHIFT: key_md_shift = false; break;
            case SDLK_LCTRL: key_md_ctrl = false; break;
            case SDLK_UP: case SDLK_DOWN: mouseMove_y = 0; break;
            case SDLK_LEFT: case SDLK_RIGHT: mouseMove_x = 0; break;
        }
    }

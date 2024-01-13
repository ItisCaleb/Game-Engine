#include <SDL2/SDL.h>
#include <stdio.h>

#include "game/game.h"

int main(int argc, char **argv) {
    int width = 1280;
    int height = 720;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SLD test",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_RESIZABLE);
    if (!window) {
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    bool running = true;
    float last_time = 0.0f;
    Game game(renderer, width, height);
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_WINDOWEVENT_RESIZED:
                    width = event.window.data1;
                    height = event.window.data2;
                    SDL_SetWindowSize(window, width, height);
                default:
                    break;
            }
        }
        float current_time = SDL_GetTicks() / 1000.0f;
        float delta = current_time - last_time;
        last_time = current_time;
        game.update(delta);

        game.render();
    }
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
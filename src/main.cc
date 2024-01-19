#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "game/game.h"
#include "utils/resource_manager.h"
#include "utils/input_manager.h"

int main(int argc, char **argv) {

    // Init everything
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        //handle error
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n",
                SDL_GetError());
        return 1;
    }
    //get screen size
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    printf("screen_width: %d, screen_height: %d\n", dm.w, dm.h);
    int width = (dm.w)/2;
    //-60 for the taskbar
    int height = (dm.h)/2-60;

    SDL_Window *window = SDL_CreateWindow("SDL test",
                                          SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                          width, height,
                                          SDL_WINDOW_RESIZABLE);

    if (!window) {
        printf("Error: Failed to open window\nSDL Error: '%s'\n",
                SDL_GetError());
        SDL_Quit();
        return 1;
    }

    int img_flag = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(img_flag) & img_flag)){
        printf("Error: SDL_image could not initialize! SDL_image Error: %s\n",
                IMG_GetError() );
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }
    
    // Game loop start
    bool running = true;
    float last_time = 0.0f;
    ResourceManager::startWorkerThread();
    Game::init(renderer, window, width, height);
    while (running) {
        // input
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
       
        
        //update the key state
        InputManager::update();
        //get delta time in milliseconds
        float current_time = SDL_GetTicks() / 1000.0f;
        float delta = current_time - last_time;
        last_time = current_time;

        // update
        Game::update(delta);

        // render
        Game::render();
    }

    Game::destroy();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

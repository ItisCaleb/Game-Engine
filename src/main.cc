#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

#include "game/game.h"
#include "entity/player.h"
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
    int width = dm.w;
    //-60 for the taskbar
    int height = dm.h-60;

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

    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("Error: SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return -1;
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
    Game::setBackground("C:\\Github\\Some-Game\\build\\test_background.png");
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

        Player *player = dynamic_cast<Player *>(Game::getPlayer());  // 假设Game类提供了获取玩家实例的方法
        if (player) {
            printf("player x: %f, y: %f\n", player->getX(), player->getY());
            Game::getCamera().update(player->getX(), player->getY());
        }


        Game::getCamera().apply(renderer);

        // render
        Game::render();

        // reset viewport
        // SDL_RenderSetViewport(renderer, NULL);
    }

    Game::destroy();
    IMG_Quit();
    SDL_Quit();
    return 0;
}

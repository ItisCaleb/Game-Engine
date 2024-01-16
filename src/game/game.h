#ifndef GAME_H_
#define GAME_H_
#include <SDL2/SDL.h>
#include <vector>
#include "entity/entity.h"

class Game {
    public:
        static void init(SDL_Renderer *renderer, SDL_Window *window, int width, int height);
        static void destroy();
        static void update(float dt);
        static void render();
        static SDL_Window* getWindow();
        static SDL_Renderer* getRenderer();
    private:
        inline static bool already_init;
        inline static SDL_Window *window;
        inline static SDL_Renderer *renderer;
        inline static std::vector<Entity*> entities;
};

#endif
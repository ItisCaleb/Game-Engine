#ifndef GAME_H_
#define GAME_H_
#include <SDL2/SDL.h>
#include <vector>
#include "entity/entity.h"

class Game {
    public:
        Game(SDL_Renderer *renderer, int width, int height);
        ~Game();
        void update(float dt);
        void render();
    private:
        SDL_Renderer *renderer;
        std::vector<Entity*> entities;
};

#endif
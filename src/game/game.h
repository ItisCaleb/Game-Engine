#ifndef GAME_H_
#define GAME_H_
#include <SDL2/SDL.h>

class Game {
    public:
        Game(SDL_Renderer *renderer);
        ~Game();
        void update(float dt);
        void render();
    private:
        SDL_Renderer *renderer;
};

#endif
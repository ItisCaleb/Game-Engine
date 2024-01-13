#ifndef ENTITY_H_
#define ENTITY_H_
#include <SDL2/SDL.h>

class Entity{
    public:
        Entity(){};
        ~Entity(){};
        virtual void update(float dt) = 0;
        virtual void render(SDL_Renderer *renderer) = 0;
};

#endif
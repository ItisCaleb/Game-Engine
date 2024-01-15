#ifndef ENTITY_H_
#define ENTITY_H_
#include <SDL2/SDL.h>

#include <vector>

#include "misc/sprite.h"

class Entity{
    public:
        Entity(int x, int y, int width, int height)
            :x(x), y(y), width(width), height(height){};
        virtual void update(float dt) = 0;
        virtual void render(SDL_Renderer *renderer) = 0;
    protected:
        std::vector<Sprite*> sprites;
        int x,y;
        int width, height;
};

#endif
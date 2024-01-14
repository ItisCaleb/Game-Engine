#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <SDL2/SDL.h>

#include <string>
#include <vector>

#include "utils/sprite.h"

class ResourceManager {
    public:
        static void init(SDL_Renderer *renderer);
        static void loadSprites(std::string resource, std::vector<Sprite*>& result);
        static Sprite* loadSprite(std::string resource);
    private:
        inline static SDL_Renderer *renderer;
};

#endif
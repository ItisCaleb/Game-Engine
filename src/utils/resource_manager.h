#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <SDL2/SDL.h>

#include <string>
#include <vector>

#include "misc/sprite.h"

class ResourceManager {
    public:
        static void init(SDL_Renderer *renderer);
        static SDL_Texture* loadTexture(std::string resource);
        static void loadSprites(std::string resource, std::vector<Sprite*>& result, int clipW, int clipH);
        static Sprite* loadSprite(std::string resource);
    private:
        inline static SDL_Renderer *renderer;
};

#endif
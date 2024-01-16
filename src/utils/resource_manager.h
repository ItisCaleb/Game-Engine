#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>

#include "misc/sprite.h"

class ResourceManager {
    public:
        static SDL_Texture* loadTexture(std::string resource);
        static void loadSprites(std::string resource, std::vector<Sprite*>& result, int clipW, int clipH);
        static Sprite* loadSprite(std::string resource);
        static nlohmann::json loadJSON(std::string resource);
};

#endif
#include "utils/resource_manager.h"

#include <SDL2/SDL_image.h>
#include <filesystem>

#include "game/game.h"

SDL_Texture* ResourceManager::loadTexture(std::string resource){
    std::filesystem::path resPath = resource;
    if( resPath.extension() == ".png" || 
        resPath.extension() == ".jpg" || 
        resPath.extension() == ".bmp"){
        SDL_Surface *surface = IMG_Load(resource.c_str());
        if(!surface){
            printf("Error: Unable to load surface from path: %s. SDL_image Error: %s\n"
                    ,resource.c_str(), IMG_GetError());
            return nullptr;
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(Game::getRenderer(), surface);
        if(!surface){
            printf("Error: Unable to create texture from: %s. SDL Error %s\n"
                    ,resource.c_str(), SDL_GetError());
            return nullptr;
        }
        SDL_FreeSurface(surface);
        return texture;
    }else{
        printf("Error: Unsupported format: \"%s\". Can't load sprite from %s",
            resPath.extension().c_str(),
            resource.c_str());
        return nullptr;
    }

}

void ResourceManager::loadSprites(std::string resource, std::vector<Sprite*> &result, int clipW, int clipH){
    std::filesystem::path resPath = resource;
    SDL_Texture *texture = ResourceManager::loadTexture(resource);
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    for(int i=0;i<w/clipW;i++){
        for(int j=0;j<h/clipH;j++){
            result.push_back(new Sprite(texture, clipW*i, clipH*j, clipW, clipH));
        }
    }
}

Sprite* ResourceManager::loadSprite(std::string resource){
    SDL_Texture *texture = ResourceManager::loadTexture(resource);
    int w, h;
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
    return new Sprite(texture, 0, 0, w, h);
}
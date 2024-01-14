#include "utils/resource_manager.h"

#include <SDL2/SDL_image.h>
#include <filesystem>

void ResourceManager::init(SDL_Renderer* renderer){
    ResourceManager::renderer = renderer;
}

void ResourceManager::loadSprites(std::string resource, std::vector<Sprite*> &result){

}

Sprite* ResourceManager::loadSprite(std::string resource){
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
        SDL_Texture *texture = SDL_CreateTextureFromSurface(ResourceManager::renderer, surface);
        if(!surface){
            printf("Error: Unable to create texture from: %s. SDL Error %s\n"
                    ,resource.c_str(), SDL_GetError());
            return nullptr;
        }
        Sprite *sprite = new Sprite(texture, 0, 0, surface->w, surface->h);
        SDL_FreeSurface(surface);
        return sprite;
    }else{
        printf("Error: Unsupported format: \"%s\". Can't load sprite from %s",
            resPath.extension().c_str(),
            resource.c_str());
        return nullptr;
    }

}
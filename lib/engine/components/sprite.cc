#include "sprite.h"
#include "engine/resource_manager.h"

Sprite::Sprite(std::string path){
    this->texture = ResourceManager::load<SDL_Texture>(path);
    this->offX = 0;
    this->offY = 0;
    SDL_QueryTexture(texture, nullptr, nullptr, &this->w, &this->h);
}
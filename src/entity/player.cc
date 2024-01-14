#include "player.h"

#include <SDL2/SDL.h>

#include "utils/resource_manager.h"

Player::Player()
:x(640), y(360), width(50), height(50){
    this->sprites.push_back(ResourceManager::loadSprite("test.png"));
}
Player::~Player() {}
void Player::update(float dt) {}
void Player::render(SDL_Renderer *renderer) {
    for(auto sprite: this->sprites){
        sprite->render(renderer, this->x, this->y);
    }
}

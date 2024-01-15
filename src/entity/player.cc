#include "player.h"

#include <SDL2/SDL.h>

#include "utils/resource_manager.h"
#include "utils/input_manager.h"

Player::Player()
:Entity(640, 360, 50, 50), hitbox(x, y, x+width, y+width), speed(200){
    this->sprites.push_back(ResourceManager::loadSprite("test.png"));
}
Player::~Player() {}
void Player::update(float dt) {
    float vx = 0, vy = 0;

    InputManager &input = InputManager::getInstance();
    if(input.isKeyPressed(InputManager::Key::A))
        vx += -this->speed;
    if(input.isKeyPressed(InputManager::Key::D))
        vx += this->speed;
    if(input.isKeyPressed(InputManager::Key::W))
        vy += -this->speed;
    if(input.isKeyPressed(InputManager::Key::S))
        vy += this->speed;
    this->x += vx * dt;
    this->y += vy * dt;
    this->hitbox.x1 = x;
    this->hitbox.x2 = x + width;
    this->hitbox.y1 = y;
    this->hitbox.y2 = y + height;
}
void Player::render(SDL_Renderer *renderer) {
    for(auto sprite: this->sprites){
        sprite->render(renderer, this->x, this->y);
    }
}

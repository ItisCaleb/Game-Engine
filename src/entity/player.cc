#include "player.h"

#include <SDL2/SDL.h>

#include "utils/resource_manager.h"
#include "utils/input_manager.h"

Player::Player()
:Entity(640, 360, 50, 50), hitbox(x, y, x+width, y+width), speed(200){
    this->sprites.push_back(ResourceManager::load<Sprite>("test.png"));
}
Player::~Player() {}
float Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

// set the screen width and height
const int SCREEN_WIDTH = 2560;
const int SCREEN_HEIGHT = 1440;
void Player::update(float dt) {

    //calculate velocity
    float vx = 0, vy = 0;
    //handle keyboard input
    if (InputManager::isKeyHold(InputManager::Key::A))
        vx += -this->speed;
    if (InputManager::isKeyHold(InputManager::Key::D))
        vx += this->speed;
    if (InputManager::isKeyHold(InputManager::Key::W))
        vy += -this->speed;
    if (InputManager::isKeyHold(InputManager::Key::S))
        vy += this->speed;
    //calculate new position
    float newX = this->x + vx * dt;
    float newY = this->y + vy * dt;

    //boundary check
    if (newX < 0) {
        newX = 0;
    } else if (newX + width > SCREEN_WIDTH) {
        newX = SCREEN_WIDTH - width;
    }

    if (newY < 0) {
        newY = 0;
    } else if (newY + height > SCREEN_HEIGHT) {
        newY = SCREEN_HEIGHT - height;
    }
    //update position
    this->x = newX;
    this->y = newY;

    //update hitbox
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


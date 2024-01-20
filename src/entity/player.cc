#include "player.h"

#include <SDL2/SDL.h>

#include "utils/resource_manager.h"
#include "utils/input_manager.h"
#include "game/game.h"

Player::Player()
:Entity(640, 360, 50, 50), hitbox(x, y, x+width, y+width), velocity(400), hurtbox(x, y, x+width, y+width){
    //this->sprites.push_back(ResourceManager::load<Sprite>("test.png"));
    this->sprites=(ResourceManager::loadSprites("asset/player/120x80_PNGSheets/_Idle.png",120,80));
    //this->width = this->sprites[0]->getWidth();
    //this->height = this->sprites[0]->getHeight();
}

Player::~Player() {}
float Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

void Player::update(float dt) {
    int maxWidth = Game::getScene()->getWidth();
    int maxHeight = Game::getScene()->getHeight();
    //calculate velocity
    float vx = 0, vy = 0;
    //handle keyboard input
    if (InputManager::isKeyHold(InputManager::Key::A))
        vx += -this->velocity;
    if (InputManager::isKeyHold(InputManager::Key::D))
        vx += this->velocity;
    if (InputManager::isKeyHold(InputManager::Key::W))
        vy += -this->velocity;
    if (InputManager::isKeyHold(InputManager::Key::S))
        vy += this->velocity;
    //calculate new position
    float newX = this->x + vx * dt;
    float newY = this->y + vy * dt;

    //boundary check
    if (newX < 0) {
        newX = 0;
    } else if (newX + width > maxWidth) {
        newX = maxWidth - width;
    }

    if (newY < 0) {
        newY = 0;
    } else if (newY + height > maxHeight) {
        newY = maxHeight - height;
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
    (*sprites)[0]->render(renderer, this->x, this->y);
    // for(auto sprite: this->sprites){
    //     sprite->render(renderer, this->x, this->y);
    // }
}


#include "player.h"

#include <SDL2/SDL.h>

#include "utils/resource_manager.h"
#include "utils/input_manager.h"
#include "game/game.h"

Player::Player()
:Entity(640, 360, 50, 50), hitbox(x, y, x+width, y+width), speed(400){
    ResourceManager::loadSprites("assets/temp/120x80_PNGSheets/_Idle.png",this->sprites);
    this->width = this->sprites[0]->getWidth() * 3;
    this->height = this->sprites[0]->getHeight() * 3;
    Game::addCollideShape(&this->hitbox, this);
}
Player::~Player() {}

void Player::update(float dt) {
    int maxWidth = Game::getScene()->getWidth();
    int maxHeight = Game::getScene()->getHeight();
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
    if (newX < -1280) {
        newX = x;
    } else if (newX + width > 3000) {
        newX = 3000 - width;
    }

    if (newY < -720) {
        newY = y;
    } else if (newY + height > 2000) {
        newY = 2000 - height;
    }

    //update position
    this->x = newX;
    this->y = newY;

    //update hitbox
    this->hitbox.x1 = x;
    this->hitbox.x2 = x + width;
    this->hitbox.y1 = y;
    this->hitbox.y2 = y + height;
    std::vector<CollideShape*> v;
    Game::getCollided(&this->hitbox, v);
    /*for(auto s:*v){
        Object *o = Game::getObjectByShape(s);
        if(o) printf("object type is %d\n",o->type);
    }*/
    //printf("collided %d objects\n",v->size());

}
void Player::render(SDL_Renderer *renderer) {
    sprites[0]->render(renderer, this->x, this->y, 3, 3);
}


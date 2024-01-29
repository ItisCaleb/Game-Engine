#include "player.h"

#include <SDL2/SDL.h>

#include "utils/resource_manager.h"
#include "utils/input_manager.h"
#include "game/game.h"

static int _idleWidth = 110;
static int _idleHeight = 80;

Player::Player()
:Entity(640, 360, 50, 50), hitbox(x, y, x+width, y+width), speed(400){
    ResourceManager::loadSprites("assets/temp/120x80_PNGSheets/_Idle.png",_idleWidth,_idleHeight, 10, 0,this->sprites);
    ResourceManager::loadSprites("assets/temp/120x80_PNGSheets/_Run.png",_idleWidth,_idleHeight, 10, 0,this->sprites);
    Game::addCollideShape(&this->hitbox, this);
    this->state = new Player::IdleState();
    this->state->enter(this);
    this->width = 60;
    this->height = 120;
}
Player::~Player() {}

void Player::update(float dt) {
    auto _state = this->state->update(this, dt);
    if(_state){
        this->state->exit(this);
        delete this->state;
        this->state = _state;
        this->state->enter(this);
    }
    
    //update hitbox
    this->hitbox.update(x,y,x+width,y+height);
    std::vector<CollideShape*> v;
    Game::getCollided(&this->hitbox, v);
    /*for(auto s:*v){
        Object *o = Game::getObjectByShape(s);
        if(o) printf("object type is %d\n",o->type);
    }*/
    //printf("collided %d objects\n",v->size());

}

void Player::render(SDL_Renderer *renderer) {
    auto sp = sprites[currentSprite];
    int x = this->x + this->width/2 - sp->getWidth()*3/2;
    int y = this->y - (sp->getHeight()*3 - this->height);
    sprites[currentSprite]->render(renderer, x, y, 3, 3, this->flip);
}


void Player::IdleState::enter(Player *instance){
    instance->getAnimator()->setIdx(0,9);
}

FSM<Player>* Player::IdleState::update(Player *instance, float dt){
    instance->getAnimator()->update(instance, dt);
    if (InputManager::isKeyHold(InputManager::WASD)){
        return new Player::RunningState;
    }
    return nullptr;
}

void Player::RunningState::enter(Player *instance){
    instance->getAnimator()->setIdx(10,19);
}

FSM<Player>* Player::RunningState::update(Player *instance, float dt){
    instance->getAnimator()->update(instance, dt);

    if (!InputManager::isKeyHold(InputManager::WASD)){
        return new Player::IdleState;
    }

    int maxWidth = Game::getScene()->getWidth();
    int maxHeight = Game::getScene()->getHeight();
    //calculate velocity
    float vx = 0, vy = 0;
    //handle keyboard input
    if (InputManager::isKeyHold(InputManager::Key::A)){
        vx += -instance->getSpeed();
        instance->setFlip(true);
    }
    if (InputManager::isKeyHold(InputManager::Key::D)){
        vx += instance->getSpeed();
        instance->setFlip(false);
    }
    if (InputManager::isKeyHold(InputManager::Key::W))
        vy += -instance->getSpeed();
    if (InputManager::isKeyHold(InputManager::Key::S))
        vy += instance->getSpeed();
    
    //calculate new position
    float x = instance->getX();
    float y = instance->getY();
    int w = instance->getWidth();
    int h = instance->getHeight();
    float newX = x + vx * dt;
    float newY = y + vy * dt;

    //boundary check
    if (newX < -1280) {
        newX = x;
    } else if (newX + w> 3000) {
        newX = 3000 - w;
    }

    if (newY < -720) {
        newY = y;
    } else if (newY + h> 2000) {
        newY = 2000 - h;
    }
    instance->setX(newX);
    instance->setY(newY);
    return nullptr;
}
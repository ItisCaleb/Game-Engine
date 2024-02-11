#include "player.h"

#include <SDL2/SDL.h>

#include <engine/resource_manager.h>
#include <engine/input_manager.h>
#include <engine/game.h>

Player::Player()
:Entity("Player",640, 360),hitbox(60,120), speed(400){
    // set flags
    this->setProps(ObjectProperty::RIGID);
    this->setProps(ObjectProperty::TRIGGER);
    this->animator.loadAnimations("assets/animations/player.json", this);

    this->attachHitbox(&this->hitbox);
    this->stateController.init(new Player::IdleState(), this);
}
Player::~Player() {}

void Player::update(float dt) {
    this->setVelocityXY(0, 0);
    this->stateController.update(this, dt);
}

void Player::onTriggerEnter(Object *obj){
    // do something
}

void Player::onTriggerStay(Object *obj){
    // do something
}

void Player::onTriggerExit(Object *obj){
    // do something
}

void Player::render(SDL_Renderer *renderer) {
    this->renderCurrentSprite(renderer, this->hitbox.w,
        this->hitbox.h, 3, 3, this->flip);
}


void Player::IdleState::enter(Player *instance){
    instance->getAnimator()->setAnimation("idle");
}

FSM<Player>* Player::IdleState::update(Player *instance, float dt){
    instance->getAnimator()->play(instance, dt);
    if (InputManager::isKeyHold(InputManager::WASD)){
        return new Player::RunningState;
    }
    return nullptr;
}

void Player::RunningState::enter(Player *instance){
    instance->getAnimator()->setAnimation("running");
}

void move(Player *instance,float dt){
  
    //boundary check
    // if (newX < -1280) {
    //     newX = x;
    // } else if (newX + w> 3000) {
    //     newX = 3000 - w;
    // }

    // if (newY < -720) {
    //     newY = y;
    // } else if (newY + h> 2000) {
    //     newY = 2000 - h;
    // }

}


FSM<Player>* Player::RunningState::update(Player *instance, float dt){
    instance->getAnimator()->play(instance, dt);

    if (!InputManager::isKeyHold(InputManager::WASD)){
        return new Player::IdleState;
    }

    //for controller 
    float axisH = 0; // ( -1 ~ 1 ) Horizontal input
    float axisV = 0; // ( -1 ~ 1 ) Vertical input
       

    //keyboard    
    if (InputManager::isKeyHold(InputManager::Key::A)){
        //vx += -instance->getSpeed();
        axisV -= 1;
        instance->setFlip(true);
    }
    if (InputManager::isKeyHold(InputManager::Key::D)){
        //vx += instance->getSpeed();
        axisV += 1;
        instance->setFlip(false);
    }
    if (InputManager::isKeyHold(InputManager::Key::W)){
        //vy += -instance->getSpeed();
        axisH -= 1;
    }
    if (InputManager::isKeyHold(InputManager::Key::S)){
        //vy += instance->getSpeed();
        axisH += 1;
        //vy+=1;
    }
    
    
    //normalize 
    float normalize = sqrt(axisH*axisH+axisV*axisV);
    float movementY=0;
    float movementX=0;
    if(normalize > 0){
        movementY = axisH/normalize;
        movementX = axisV/normalize;
    }else{
        movementY = 0;
        movementX = 0;
    }

    float speed = instance->getSpeed();

    instance->setVelocityXY(movementX * speed, movementY * speed);

    return nullptr;
}

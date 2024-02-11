#include "player.h"
#include "player_controller.h"

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
    
    this->isMove = false;
    this->isAttack = false;
    this->_lastTick = SDL_GetTicks();
    this->comboCnt = 0;
}
Player::~Player() {}

void move(Player *instance,float dt);
void attack(Player *instance,float dt);

void Player::update(float dt) {
    if(!isAttack){
        move(this,dt);
        attack(this,dt);
    }
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

void attack(Player *instance,float dt){
    if(InputManager::isKeyDown(InputManager::Key::J)){
        //printf("j");
        instance->isAttack = true;
    }
}

void move(Player *instance,float dt){
    if(instance->isAttack)return;
    instance->isMove = true;

    //for controller 
    float axisH = 0; // ( -1 ~ 1 ) Horizontal input
    float axisV = 0; // ( -1 ~ 1 ) Vertical input
       
    //keyboard    
    if (InputManager::isKeyHold(InputManager::Key::A)){
        axisV -= 1;
    }
    if (InputManager::isKeyHold(InputManager::Key::D)){
        axisV += 1;
    }
    if (InputManager::isKeyHold(InputManager::Key::W)){
        axisH -= 1;
    }
    if (InputManager::isKeyHold(InputManager::Key::S)){
        axisH += 1;
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
        instance->isMove = 0;
    }

    float speed = instance->getSpeed();

    instance->setVelocityXY(movementX*speed,movementY*speed);
}

void Player::IdleState::enter(Player *instance){
    instance->getAnimator()->setAnimation("idle");
}

FSM<Player>* Player::IdleState::update(Player *instance, float dt){
    instance->getAnimator()->play(instance, dt);
    if(instance->isAttack) return new Player::AttackingState;
    if(instance->isMove)   return new Player::RunningState;
    
    return nullptr;
}

void Player::RunningState::enter(Player *instance){
    instance->getAnimator()->setAnimation("running");
}

FSM<Player>* Player::RunningState::update(Player *instance, float dt){
    instance->getAnimator()->play(instance, dt);
    if(instance->isAttack) return new Player::AttackingState;
    if(!instance->isMove)return new Player::IdleState;
    if (InputManager::isKeyHold(InputManager::Key::A)){
        instance->setFlip(true);
    }
    if (InputManager::isKeyHold(InputManager::Key::D)){
        instance->setFlip(false);
    }
    return nullptr;
}

void Player::AttackingState::enter(Player *instance){
    instance->getAnimator()->setAnimation("attack1");
}

FSM<Player>* Player::AttackingState::update(Player *instance, float dt){
    instance->getAnimator()->play(instance, dt);
    if(instance->getAnimator()->isEnding()){
        instance->isAttack = false;
        return new Player::IdleState;
    }
    return nullptr;
}


#include "player.h"
#include "player_controller.h"

#include <SDL2/SDL.h>
#include <engine/resource_manager.h>
#include <engine/input_manager.h>
#include <engine/game.h>
#include <engine/timer.h>

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
    this->enAttack = true;
    this->comboCnt = 0;
    this->comboTimer.start();
    this->comboResetTime = 400; // 不要問
    
}
Player::~Player() {}

void move(Player *instance,float dt);
void attack(Player *instance,float dt);

void Player::update(float dt) {
    this->setVelocityXY(0, 0);
    move(this,dt);
    attack(this,dt);
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
    if(InputManager::isKeyDown(InputManager::Key::J) && instance->enAttack){
        instance->enAttack = false;
        instance->isAttack = true;
        instance->comboCnt++;
        if(instance->comboCnt>2)instance->comboCnt=0;
        instance->comboTimer.restart();
    }
    if(instance->comboTimer.getTicks() >= instance->comboResetTime){
        //printf("0\n");
        instance->comboCnt = 0;
    }
}

void attackOver(Player *instance){
    instance->isAttack = false;
}

void move(Player *instance,float dt){
    if(instance->isAttack)return;
    instance->isMove = true;

    //turn
    if (InputManager::isKeyHold(InputManager::Key::A)){
        instance->setFlip(true);
    }
    if (InputManager::isKeyHold(InputManager::Key::D)){
        instance->setFlip(false);
    }

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

    //turn
    if (InputManager::isKeyHold(InputManager::Key::A)){
        instance->setFlip(true);
    }
    if (InputManager::isKeyHold(InputManager::Key::D)){
        instance->setFlip(false);
    }
}

void Player::IdleState::enter(Player *instance){
    instance->getAnimator()->setAnimation("idle");
}

FSM<Player>* Player::tryUpdate(){
    if(isAttack){
        if(comboCnt==2)return new Player::Attack2State;
        else return new Player::Attack1State;
    }
    return nullptr;
}

FSM<Player>* Player::IdleState::update(Player *instance, float dt){
    instance->enAttack = true;
    instance->getAnimator()->play(instance, dt);
    auto tryAttack = instance->tryUpdate();
    if(tryAttack)return tryAttack;
    if(instance->isMove)return new Player::RunningState;
    return nullptr;
}

void Player::RunningState::enter(Player *instance){
    instance->getAnimator()->setAnimation("running");
}

FSM<Player>* Player::RunningState::update(Player *instance, float dt){
    instance->getAnimator()->play(instance, dt);
    if(instance->isAttack) return new Player::Attack1State;
    if(!instance->isMove)return new Player::IdleState;
    return nullptr;
}

void Player::Attack1State::enter(Player *instance){
    instance->getAnimator()->setAnimation("attack1");
}

FSM<Player>* Player::Attack1State::update(Player *instance, float dt){
    instance->getAnimator()->play(instance, dt);
    if(instance->getAnimator()->getCurrentAnim()>=1){
        instance->enAttack = true;
    }
    if(instance->getAnimator()->getCurrentAnim()>=2){
        if(instance->comboCnt==2)return new Player::Attack2State;
    }
    if(instance->getAnimator()->isEnding()){
        //if(instance->comboCnt==2)return new Player::Attack2State;
        instance->isAttack = false;
        return new Player::IdleState;
    }
    return nullptr;
}

void Player::Attack2State::enter(Player *instance){
    instance->getAnimator()->setAnimation("attack2");
}

FSM<Player>* Player::Attack2State::update(Player *instance, float dt){
    instance->getAnimator()->play(instance, dt);
    if(instance->getAnimator()->isEnding()){
        instance->enAttack = true;
        instance->isAttack = false;
        return new Player::IdleState;
    }
    return nullptr;
}


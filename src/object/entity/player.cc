#include "player.h"
#include "player_controller.h"

#include <SDL2/SDL.h>
#include <engine/resource_manager.h>
#include <engine/input_manager.h>
#include <engine/game.h>

static int _idleWidth = 120;
static int _idleHeight = 80;

PlayerController pp;

Player::Player()
:Entity("Player",640, 360, 50, 50), hitbox(x, y, x+width, y+width), speed(400){
    int r = ResourceManager::loadSprites("assets/temp/120x80_PNGSheets/_Idle.png",_idleWidth,_idleHeight, 0, 0,this->sprites);
    this->animator.addAnimation("idle", r);
    r = ResourceManager::loadSprites("assets/temp/120x80_PNGSheets/_Run.png",_idleWidth,_idleHeight, 0, 0,this->sprites);
    this->animator.addAnimation("running", r);
    r = ResourceManager::loadSprites("assets/temp/120x80_PNGSheets/_TurnAround.png",_idleWidth,_idleHeight, 0, 0,this->sprites);
    this->animator.addAnimation("turnAround", r);
    r = ResourceManager::loadSprites("assets/temp/120x80_PNGSheets/_Attack.png",_idleWidth,_idleHeight, 0, 0,this->sprites);
    this->animator.addAnimation("attack1", r);
    r = ResourceManager::loadSprites("assets/temp/120x80_PNGSheets/_Attack2.png",_idleWidth,_idleHeight, 0, 0,this->sprites);
    this->animator.addAnimation("attack2", r);

    Game::getScene()->addCollideShape(&this->hitbox);
    this->state = new Player::IdleState();
    this->state->enter(this);
    this->width = 60;
    this->height = 120;

    this->isMove = false;
    this->isAttack = false;
    this->_lastTick = SDL_GetTicks();
    this->comboCnt = 0;
}
Player::~Player() {}

void move(Player *instance,float dt);
void attack(Player *instance,float dt);

void Player::update(float dt) {
    
    auto _state = this->state->update(this, dt);
    if(_state){
        this->state->exit(this);
        delete this->state;
        this->state = _state;
        this->state->enter(this);
    }
    
    //update hitbox
    std::vector<CollideShape*> v;
    Game::getScene()->getCollided(&this->hitbox, v);
    /*for(auto s:*v){
        Object *o = Game::getObjectByShape(s);
        if(o) printf("object type is %d\n",o->type);
    }*/
    //printf("collided %d objects\n",v->size());
    if(SDL_GetTicks()-this->_lastTick>1000){
        _lastTick = SDL_GetTicks();
        printf("h");
    }

    move(this,dt);
    attack(this,dt);
}

void Player::render(SDL_Renderer *renderer) {
    auto sp = sprites[currentSprite];
    int x = this->x + this->hitbox.w/2 - sp->getWidth()*3/2;
    int y = this->y - (sp->getHeight()*3 - this->hitbox.h);
    sprites[currentSprite]->render(renderer, x, y, 3, 3, this->flip);
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

    instance->setX(newX);
    instance->setY(newY);
}

void Player::IdleState::enter(Player *instance){
    instance->getAnimator()->playAnimation("idle");
}

FSM<Player>* Player::IdleState::update(Player *instance, float dt){
    instance->getAnimator()->update(instance, dt);
    if(instance->isAttack) return new Player::AttackingState;
    if(instance->isMove)   return new Player::RunningState;
    
    return nullptr;
}

void Player::RunningState::enter(Player *instance){
    instance->getAnimator()->playAnimation("running");
}

FSM<Player>* Player::RunningState::update(Player *instance, float dt){
    instance->getAnimator()->update(instance, dt);
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
    instance->getAnimator()->playAnimation("attack1");
}

FSM<Player>* Player::AttackingState::update(Player *instance, float dt){
    instance->getAnimator()->update(instance, dt);
    if(instance->getAnimator()->getCurrentAnim()==3){
        instance->isAttack = false;
        return new Player::IdleState;
    }
    return nullptr;
}


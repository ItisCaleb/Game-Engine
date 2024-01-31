#include "player.h"

#include <SDL2/SDL.h>

#include <engine/resource_manager.h>
#include <engine/input_manager.h>
#include <engine/game.h>

static int _idleWidth = 110;
static int _idleHeight = 80;

Player::Player()
:Entity("Player",640, 360, 50, 50), hitbox(x, y, x+width, y+width), speed(400){
    int r = ResourceManager::loadSprites("assets/temp/120x80_PNGSheets/_Idle.png",_idleWidth,_idleHeight, 10, 0,this->sprites);
    this->animator.addAnimation("idle", r);
    r = ResourceManager::loadSprites("assets/temp/120x80_PNGSheets/_Run.png",_idleWidth,_idleHeight, 10, 0,this->sprites);
    this->animator.addAnimation("running", r);

    Game::getScene()->addCollideShape(&this->hitbox, this);
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
    Game::getScene()->getCollided(&this->hitbox, v);
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
    instance->getAnimator()->playAnimation("idle");
}

FSM<Player>* Player::IdleState::update(Player *instance, float dt){
    instance->getAnimator()->update(instance, dt);
    if (InputManager::isKeyHold(InputManager::WASD)){
        return new Player::RunningState;
    }
    return nullptr;
}

void Player::RunningState::enter(Player *instance){
    instance->getAnimator()->playAnimation("running");
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
    instance->getAnimator()->update(instance, dt);

    if (!InputManager::isKeyHold(InputManager::WASD)){
        return new Player::IdleState;
    }

    int maxWidth = Game::getScene()->getWidth();
    int maxHeight = Game::getScene()->getHeight();
    //calculate velocity


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
    float newX = instance->getX() + movementX * speed * dt;
    float newY = instance->getY() + movementY * speed * dt;

    instance->setX(newX);
    instance->setY(newY);

    return nullptr;
}

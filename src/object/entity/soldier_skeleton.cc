#include "soldier_skeleton.h"
#include <SDL2/SDL.h>
#include <engine/game.h>
#include <engine/input_manager.h>
#include <engine/resource_manager.h>

static int _idleWidth = 24;
static int _idleHeight = 32;

Skeleton::Skeleton()
    : Entity(300, 360, 50, 50), hitbox(x, y, x + width, y + width), speed(400) {
    //0~11 idle
    ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Idle.png", 24, 32, 0, 0, this->sprites);
    //11~23 walk
    ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Walk.png", 22, 33, 0, 0, this->sprites);
    //24~27 react
    ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_React.png", 22, 32, 0, 0, this->sprites);
    //28~45 attack
    ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Attack.png", 33, 37, 0, 0, this->sprites);
    //46~53 hit
    ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Hit.png", 30, 32, 0, 0, this->sprites);
    //54~68 dead
    ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Dead.png", 33, 32, 0, 0, this->sprites);
    
    if(this->sprites.size() == 0){
        printf("Error: Skeleton sprite not loaded\n");
    }   
    Game::addCollideShape(&this->hitbox, this);
    this->state = new Skeleton::IdleState();
    this->currentState = SkeletonState::Idle;
    this->state->enter(this);
    this->width = 60;
    this->height = 120;

    // init state
    this->patrolDirection = Direction::Left;
}
Skeleton::~Skeleton() {}

void Skeleton::update(float dt) {
    auto _state = this->state->update(this, dt);
    if (_state) {
        this->state->exit(this);
        delete this->state;
        this->state = _state;
        this->state->enter(this);
    }

    // update hitbox
    this->hitbox.update(x, y, x + width, y + height);
    std::vector<CollideShape *> v;
    Game::getCollided(&this->hitbox, v);
    /*for(auto s:*v){
        Object *o = Game::getObjectByShape(s);
        if(o) printf("object type is %d\n",o->type);
    }*/
    // printf("collided %d objects\n",v->size());
}

void Skeleton::render(SDL_Renderer *renderer) {
    auto sp = sprites[currentSprite];
    int x = this->x + this->width / 2 - sp->getWidth() * 3 / 2;
    int y = this->y - (sp->getHeight() * 3 - this->height);
    sprites[currentSprite]->render(renderer, x, y, 3, 3, this->flip);
}

void Skeleton::IdleState::enter(Skeleton *instance) {
    instance->getAnimator()->setIdx(0, 10);   
    printf("current state is %d\n",instance->currentState); 
    switch (instance->currentState){
        case SkeletonState::Patrol:
            instance->idleTimer = 3.0f;
            break;
        default:
            printf("idle state\n");
            instance->idleTimer = 5.0f;
            break;
    }
}

FSM<Skeleton> *Skeleton::IdleState::update(Skeleton *instance, float dt) {
    instance->getAnimator()->update(instance, dt);
    printf("idle timer is %f\n",instance->idleTimer);
    if(instance->idleTimer > 0){
        instance->idleTimer -= dt;
    }else{
        return new Skeleton::PatrolState;
    }
    return nullptr;
}

void Skeleton::PatrolState::enter(Skeleton *instance) {
    instance->getAnimator()->setIdx(11, 23);
    instance->currentState = SkeletonState::Patrol;
    
    instance->patrolTimer = 5.0f;       // 設置巡邏計時器為5秒
    instance->patrolSpeed = 20.0f;     // 設置巡邏速度為100像素/秒
}
FSM<Skeleton> *Skeleton::PatrolState::update(Skeleton *instance, float dt) {
    instance->getAnimator()->update(instance, dt);
    if (instance->patrolTimer > 0) {
        // 執行巡邏
        float speed = instance->patrolSpeed;
        if (instance->patrolDirection == Direction::Left) {
            instance->setX(instance->getX() - speed * dt);
            instance->setFlip(true);  // 假設向左走時需要翻轉圖像
        } else {
            instance->setX(instance->getX() + speed * dt);
            instance->setFlip(false);  // 假設向右走時不需要翻轉圖像
        }
        instance->patrolTimer -= dt;
    } else {
        // 切換方向並重置計時器
        instance->patrolTimer = 5.0f;
        if (instance->patrolDirection == Direction::Left) {
            instance->patrolDirection = Direction::Right;
            instance->currentState = SkeletonState::Idle;
            return new Skeleton::IdleState;
        } else {
            instance->patrolDirection = Direction::Left;
            instance->currentState = SkeletonState::Idle;   
            return new Skeleton::IdleState;
        }
    }
    return nullptr;
}

void Skeleton::RunningState::enter(Skeleton *instance) {
    instance->getAnimator()->setIdx(11, 23);
}

FSM<Skeleton> *Skeleton::RunningState::update(Skeleton *instance, float dt) {
    instance->getAnimator()->update(instance, dt);

    if (!InputManager::isKeyHold(InputManager::WASD)) {
        return new Skeleton::IdleState;
    }

    int maxWidth = Game::getScene()->getWidth();
    int maxHeight = Game::getScene()->getHeight();
    // calculate velocity
    float vx = 0, vy = 0;
    // handle keyboard input
    if (InputManager::isKeyHold(InputManager::Key::A)) {
        vx += -instance->getSpeed();
        instance->setFlip(true);
    }
    if (InputManager::isKeyHold(InputManager::Key::D)) {
        vx += instance->getSpeed();
        instance->setFlip(false);
    }
    if (InputManager::isKeyHold(InputManager::Key::W))
        vy += -instance->getSpeed();
    if (InputManager::isKeyHold(InputManager::Key::S))
        vy += instance->getSpeed();

    // calculate new position
    float x = instance->getX();
    float y = instance->getY();
    int w = instance->getWidth();
    int h = instance->getHeight();
    float newX = x + vx * dt;
    float newY = y + vy * dt;

    // boundary check
    if (newX < -1280) {
        newX = x;
    } else if (newX + w > 3000) {
        newX = 3000 - w;
    }
    if (newY < -720) {
        newY = y;
    } else if (newY + h > 2000) {
        newY = 2000 - h;
    }
    instance->setX(newX);
    instance->setY(newY);
    return nullptr;
}
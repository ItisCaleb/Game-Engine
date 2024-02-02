#include "soldier_skeleton.h"
#include "player.h"
#include <SDL2/SDL.h>
#include <engine/game.h>
#include <engine/input_manager.h>
#include <engine/resource_manager.h>

static int _idleWidth = 24;
static int _idleHeight = 32;

Skeleton::Skeleton()
    : Entity("Skeleton",300, 360, 50, 50), hitbox(x, y, x + width, y + width), speed(400) {
    //0~11 idle
    int r = ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Idle.png", 24, 32, 0, 0, this->sprites);
    this->animator.addAnimation("idle",r);
    //11~23 walk
    r = ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Walk.png", 22, 33, 0, 0, this->sprites);
    this->animator.addAnimation("patrol",r);
    //24~27 react
    r = ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_React.png", 22, 32, 0, 0, this->sprites);
    this->animator.addAnimation("reaching",r);
    //28~45 attack
    r = ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Attack.png", 33, 37, 0, 0, this->sprites);
    this->animator.addAnimation("attacking",r);
    //46~53 hit
    r = ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Hit.png", 30, 32, 0, 0, this->sprites);
    this->animator.addAnimation("injured",r);
    //54~68 dead
    r = ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Dead.png", 33, 32, 0, 0, this->sprites);
    this->animator.addAnimation("dying",r);
    
    if(this->sprites.size() == 0){
        printf("Error: Skeleton sprite not loaded\n");
    }   
    Game::getScene()->addCollideShape(&this->hitbox, this);
    this->state = new Skeleton::IdleState();
    this->currentState = SkeletonState::Idle;
    this->state->enter(this);
    this->width = 60;
    this->height = 120;
    Player* player = dynamic_cast<Player*>(Game::getScene()->getObjectByTag("Player"));
    this->playerX = player->getX();
    this->playerY = player->getY();
    
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
    Game::getScene()->getCollided(&this->hitbox, v);
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
    instance->getAnimator()->setAnimation("idle");
    switch (instance->currentState){
        case SkeletonState::Patrol:
            instance->idleTimer = 3.0f;
            break;
        default:
            instance->idleTimer = 5.0f;
            break;
    }
}

FSM<Skeleton> *Skeleton::IdleState::update(Skeleton *instance, float dt) {
    instance->getAnimator()->play(instance, dt);
    
    if(instance->idleTimer > 0){
        instance->idleTimer -= dt;
    }else if(instance->idleTimer <= 0){
        return new Skeleton::PatrolState;
    }
    return nullptr;
}

void Skeleton::PatrolState::enter(Skeleton *instance) {
    instance->getAnimator()->setAnimation("patrol");
    instance->currentState = SkeletonState::Patrol;
    
    instance->patrolTimer = 5.0f;       // 設置巡邏計時器為5秒
    instance->patrolSpeed = 20.0f;     // 設置巡邏速度為100像素/秒
}
FSM<Skeleton> *Skeleton::PatrolState::update(Skeleton *instance, float dt) {
    instance->getAnimator()->play(instance, dt);
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

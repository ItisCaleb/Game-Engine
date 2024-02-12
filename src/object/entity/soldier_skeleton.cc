#include "soldier_skeleton.h"
#include "player.h"
#include <SDL2/SDL.h>
#include <engine/game.h>
#include <engine/input_manager.h>
#include <engine/resource_manager.h>

static int _idleWidth = 24;
static int _idleHeight = 32;

Skeleton::Skeleton()
    : Entity("Skeleton",300, 360), hitbox(60,120), speed(400) {
    this->setProps(ObjectProperty::RIGID);
    this->setProps(ObjectProperty::TRIGGER);
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
    r = ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Attack.png", 43, 37, 0, 0, this->sprites);
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
    this->attachHitbox(&this->hitbox);
    this->state = new Skeleton::IdleState();
    this->currentState = SkeletonState::Idle;
    this->state->enter(this);
 
    Player* player = dynamic_cast<Player*>(Game::getScene()->getObjectByTag("Player"));
    this->playerX = player->getX();
    this->playerY = player->getY();
    this->domainDistance = 90000;
    // init state
    this->patrolDirection = Direction::Left;
    this->isAttacking = false;
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
    int x = this->x + this->hitbox.w / 2 - sp->getWidth() * 3 / 2;
    int y = this->y - (sp->getHeight() * 3 - this->hitbox.h);
    sprites[currentSprite]->render(renderer, x, y, 3, 3, this->flip);
}

void Skeleton::onTrigger(CollideShape *shape) {
    this->isAttacking = true;
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

    Player *player = dynamic_cast<Player *>(Game::getScene()->getObjectByTag("Player"));
    if(instance->isAttacking){
        return new Skeleton::AttackingState;
    }
    if (player) {
        instance->playerX = player->getX();
        instance->playerY = player->getY();
    }
    float dx = instance->playerX - instance->getX();
    float dy = instance->playerY - instance->getY();
    float distance = dx * dx + dy * dy;
    if (distance > 10 && distance <= instance->domainDistance) {
        return new Skeleton::ReachingState;
    } else if (instance->idleTimer > 0) {
        instance->idleTimer -= dt;
    } else if (instance->idleTimer <= 0) {
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
    
    Player *player = dynamic_cast<Player *>(Game::getScene()->getObjectByTag("Player"));
    if (instance->isAttacking) {
        return new Skeleton::AttackingState;
    }
    if (player) {
        instance->playerX = player->getX();
        instance->playerY = player->getY();
    }
    float dx = instance->playerX - instance->getX();
    float dy = instance->playerY - instance->getY();
    float distance = dx * dx + dy * dy;
    if (distance > 10 && distance <= instance->domainDistance) {
        return new Skeleton::ReachingState;
    } else if (instance->patrolTimer > 0) {
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

void Skeleton::ReachingState::enter(Skeleton *instance) {
    instance->getAnimator()->setAnimation("reaching");
    instance->currentState = SkeletonState::Reaching;
    instance->reachingTimer = 0.08f;
}
FSM<Skeleton> *Skeleton::ReachingState::update(Skeleton *instance, float dt) {
    instance->getAnimator()->play(instance, dt);
    if (instance->isAttacking) {
        return new Skeleton::AttackingState;
    }
    if (instance->reachingTimer > 0) {
        printf("timer: %f\n", instance->reachingTimer);  // 這裡做一些攻擊動作
        printf("dt: %f\n", dt);  // 這裡做一些攻擊動作              
        instance->reachingTimer -= dt;
    } else if (instance->reachingTimer <= 0) {
        return new Skeleton::PursuingState;
    }
    return nullptr;
}

void Skeleton::PursuingState::enter(Skeleton *instance) {
    instance->getAnimator()->setAnimation("patrol");
    instance->currentState = SkeletonState::Pursuing;
    instance->patrolSpeed = 60.0f;
}

FSM<Skeleton> *Skeleton::PursuingState::update(Skeleton *instance, float dt) {
    instance->getAnimator()->play(instance, dt);
    instance->currentState = SkeletonState::Idle;
    Player *player = dynamic_cast<Player *>(Game::getScene()->getObjectByTag("Player"));
    if (instance->isAttacking) {
        return new Skeleton::AttackingState;
    }
    if (player) {
        instance->playerX = player->getX();
        instance->playerY = player->getY();
    }
    float dx = instance->playerX - instance->getX();
    float dy = instance->playerY - instance->getY();
    float distance = dx * dx + dy * dy;
    if ( distance > 10 && distance <= instance->domainDistance ) {
        float speed = instance->getSpeed();
        float angle = atan2(dy, dx);
        instance->setX(instance->getX() + cos(angle) * instance->patrolSpeed * dt);
        instance->setY(instance->getY() + sin(angle) * instance->patrolSpeed * dt);
        // 根據玩家位置設置圖像翻轉
        instance->setFlip(dx < 0);
    } else if (distance <= 10 || distance > instance->domainDistance) {
        instance->isAttacking = false;
        // 如果玩家超出範圍，可以切換回其他狀態
        return new Skeleton::IdleState();  // 這裡假設返回到Idle狀態，根據需要調整
    }
    // return new Skeleton::IdleState;
    return nullptr;
}

void Skeleton::AttackingState::enter(Skeleton *instance) {
    instance->getAnimator()->setAnimation("attacking");
    instance->currentState = SkeletonState::Attacking;
    instance->patrolSpeed = 60.0f;
}

FSM<Skeleton> *Skeleton::AttackingState::update(Skeleton *instance, float dt) {
    instance->getAnimator()->play(instance, dt);
    instance->currentState = SkeletonState::Idle;
    Player *player = dynamic_cast<Player *>(Game::getScene()->getObjectByTag("Player"));
    if (player) {
        instance->playerX = player->getX();
        instance->playerY = player->getY();
    }
    float dx = instance->playerX - instance->getX();
    float dy = instance->playerY - instance->getY();
    float distance = dx * dx + dy * dy;
    if (distance > 10 && distance <= instance->domainDistance) {
        float speed = instance->getSpeed();
        float angle = atan2(dy, dx);
        instance->setX(instance->getX() + cos(angle) * instance->patrolSpeed * dt);
        instance->setY(instance->getY() + sin(angle) * instance->patrolSpeed * dt);
        // 根據玩家位置設置圖像翻轉
        instance->setFlip(dx < 0);
    } else if (distance <= 10 || distance > instance->domainDistance) {
        instance->isAttacking = false;
        // 如果玩家超出範圍，可以切換回其他狀態
        return new Skeleton::IdleState();  // 這裡假設返回到Idle狀態，根據需要調整
    }
    // return new Skeleton::IdleState;
    return nullptr;
}

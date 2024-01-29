#include <SDL2/SDL.h>

#include "game/game.h"
#include "soldier_skeleton.h"
#include "utils/input_manager.h"
#include "utils/resource_manager.h"

static int _idleWidth = 24;
static int _idleHeight = 32;

Skeleton::Skeleton()
    : Entity(300, 360, 50, 50), hitbox(x, y, x + width, y + width), speed(400) {
    ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Idle.png", _idleWidth, _idleHeight, 0, 0, this->sprites);
    ResourceManager::loadSprites("assets/temp/Skeleton/Sprite_Sheets/Skeleton_Walk.png", 22, 33, 0, 0, this->sprites);
    if(this->sprites.size() == 0){
        printf("Error: Skeleton sprite not loaded\n");
    }   
    Game::addCollideShape(&this->hitbox, this);
    this->state = new Skeleton::IdleState();
    this->state->enter(this);
    this->width = 60;
    this->height = 120;
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
}

FSM<Skeleton> *Skeleton::IdleState::update(Skeleton *instance, float dt) {
    instance->getAnimator()->update(instance, dt);
    if (InputManager::isKeyHold(InputManager::WASD)) {
        return new Skeleton::RunningState;
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
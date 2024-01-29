#ifndef SOLDIER_SKELETON_H_
#define SOLDIER_SKELETON_H_
#include "entity.h"
#include "misc/animator.h"
#include "misc/collide_shape.h"
#include "misc/fsm.h"

class Skeleton : public Entity {
   public:
    Skeleton();
    ~Skeleton();
    void update(float dt);
    void render(SDL_Renderer *renderer);
    Animator *getAnimator() {
        return &this->animator;
    }
    int getSpeed() {
        return this->speed;
    }
    bool setFlip(bool flip) {
        this->flip = flip;
    }

   private:
    BoxCollideShape hitbox;
    FSM<Skeleton> *state;
    Animator animator;
    int speed;
    bool flip;

    class IdleState : public FSM<Skeleton> {
       public:
        void enter(Skeleton *instance);
        FSM<Skeleton> *update(Skeleton *instance, float dt);
        void exit(Skeleton *instance) {}
    };

    class RunningState : public FSM<Skeleton> {
       public:
        void enter(Skeleton *instance);
        FSM<Skeleton> *update(Skeleton *instance, float dt);
        void exit(Skeleton *instance) {}
    };
};

#endif
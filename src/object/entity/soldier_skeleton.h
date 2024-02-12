#ifndef SOLDIER_SKELETON_H_
#define SOLDIER_SKELETON_H_
#include <engine/animator.h>
#include <engine/collide_shape.h>
#include <engine/entity.h>
#include <engine/fsm.h>

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
    void onTrigger(CollideShape *shape);

   private:
    BoxCollideShape hitbox;
    FSM<Skeleton> *state;
    Animator animator;
    int speed;
    bool flip;
    bool isAttacking;
    float idleTimer;
    float patrolTimer;
    float patrolSpeed;
    float domainDistance;
    float reachingTimer;
    enum class Direction { Left,
                                 Right };
    enum class SkeletonState { Idle,
                               Patrol,
                               Reaching,
                               Pursuing,
                               Attacking,
                               Injured,
                               Dying };
    SkeletonState currentState;
    Direction patrolDirection;
    float playerX, playerY;
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
    class PatrolState : public FSM<Skeleton> {
       public:
        void enter(Skeleton *instance);
        FSM<Skeleton> *update(Skeleton *instance, float dt);
        void exit(Skeleton *instance) {}
    };
    class ReachingState : public FSM<Skeleton> {
       public:
        void enter(Skeleton *instance);
        FSM<Skeleton> *update(Skeleton *instance, float dt);
        void exit(Skeleton *instance) {}
    };
    class PursuingState : public FSM<Skeleton> {
       public:
        void enter(Skeleton *instance);
        FSM<Skeleton> *update(Skeleton *instance, float dt);
        void exit(Skeleton *instance) {}
    };
    class AttackingState : public FSM<Skeleton> {
       public:
        void enter(Skeleton *instance);
        FSM<Skeleton> *update(Skeleton *instance, float dt);
        void exit(Skeleton *instance) {}
    };
    class injuredState : public FSM<Skeleton> {
       public:
        void enter(Skeleton *instance);
        FSM<Skeleton> *update(Skeleton *instance, float dt);
        void exit(Skeleton *instance) {}
    };
    class DyingState : public FSM<Skeleton> {
       public:
        void enter(Skeleton *instance);
        FSM<Skeleton> *update(Skeleton *instance, float dt);
        void exit(Skeleton *instance) {}
    };

};

#endif
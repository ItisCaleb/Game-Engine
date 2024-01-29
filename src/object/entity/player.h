#ifndef PLAYER_H_
#define PLAYER_H_
#include "entity.h"
#include "misc/collide_shape.h"
#include "misc/fsm.h"
#include "misc/animator.h"


class Player : public Entity{
    public:
        Player();
        ~Player();
        void update(float dt);
        void render(SDL_Renderer *renderer);
        Animator *getAnimator(){
            return &this->animator;
        }
        float getSpeed(){
            return this->speed;
        }
        bool setFlip(bool flip){
            this->flip = flip;
        }
    private:
        BoxCollideShape hitbox;
        FSM<Player> *state;
        Animator animator;
        float speed;
        float velocity;
        float maxSpeed;
        bool flip;


    class IdleState: public FSM<Player>{
        public:
            void enter(Player *instance);
            FSM<Player>* update(Player *instance, float dt);
            void exit(Player *instance){}
    };

    class RunningState: public FSM<Player>{
        public:
            void enter(Player *instance);
            FSM<Player>* update(Player *instance, float dt);
            void exit(Player *instance){}
    };
};

#endif
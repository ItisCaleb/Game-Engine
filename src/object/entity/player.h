#ifndef PLAYER_H_
#define PLAYER_H_
#include <engine/entity.h>
#include <engine/collide_shape.h>
#include <engine/fsm.h>
#include <engine/animator.h>


class Player : public Entity{
    public:
        Player();
        ~Player();
        void update(float dt);
        void render(SDL_Renderer *renderer);
        Animator *getAnimator(){
            return &this->animator;
        }
        float getWidth(){
            return this->hitbox.w;
        }
        float getHeight(){
            return this->hitbox.h;
        }
        float getSpeed(){
            return this->speed;
        }
        void setFlip(bool flip){
            this->flip = flip;
        }
        void onTrigger(CollideShape *shape);
    private:
        FSM<Player> *state;
        Animator animator;
        BoxCollideShape hitbox;
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
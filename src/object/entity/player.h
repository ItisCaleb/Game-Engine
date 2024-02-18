#ifndef PLAYER_H_
#define PLAYER_H_
#include <engine/entity.h>
#include <engine/collide_shape.h>
#include <engine/fsm.h>
#include <engine/animator.h>
#include <engine/timer.h>


class Player : public Entity{
    public:
        Player();
        ~Player();
        void update(float dt);
        void render(Renderer *renderer);
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
        bool isMove;
        bool isAttack; // is attacking
        bool isFlip;
        bool enAttack; // enable to input Attack
        int comboCnt; // default 0  c1 c2
        int comboResetTime; // clear comboCnt to 0
        Timer comboTimer;

        BoxCollideShape* getHitbox(){
            return &hitbox;
        }
        void onTriggerEnter(Object *obj);
        void onTriggerStay(Object *obj);

        void onTriggerExit(Object *obj);

        FSM<Player>* tryUpdate();

    private:

        FSMController<Player> stateController;
        Animator animator;
        BoxCollideShape hitbox;
        float speed;
        float velocity;
        float maxSpeed;
        bool flip = false;

        

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

    class Attack1State: public FSM<Player>{
        public:
            void enter(Player *instance);
            FSM<Player>* update(Player *instance, float dt);
            void exit(Player *instance){}
    };

    class Attack2State: public FSM<Player>{
        public:
            void enter(Player *instance);
            FSM<Player>* update(Player *instance, float dt);
            void exit(Player *instance){}
    };

};

#endif
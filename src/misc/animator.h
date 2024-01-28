#ifndef ANIMATOR_H_
#define ANIMATOR_H_


#include "misc/fsm.h"

#include "object/entity/entity.h"
#include "utils/timer.h"

class Animator: public FSM<Entity>{
        public:
            void enter(Entity *instance){
                this->timer.start();
                this->anim = startIdx;
            }
            FSM<Entity>* update(Entity *instance, float dt){
                if(timer.getTicks() >= 84){
                    instance->chooseCurrentSprite(anim);
                    this->anim++;
                    if(this->anim > this->endIdx) 
                        this->anim = this->startIdx;
                    this->timer.restart();
                }
                return nullptr;
            }
            void exit(Entity *instance){}

            void setIdx(int startIdx, int endIdx){
                this->startIdx = startIdx;
                this->endIdx = endIdx;
                this->anim = startIdx;
            }
        private:
            Timer timer;
            int startIdx, endIdx;
            int anim;
    };

#endif
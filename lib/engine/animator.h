#ifndef ANIMATOR_H_
#define ANIMATOR_H_


#include "fsm.h"

#include <unordered_map>

#include "engine/entity.h"
#include "engine/timer.h"

class AnimeProperty{
    public:
        friend class Animator;
        int getLength(){
            return endIdx - startIdx;
        }
    private:
        AnimeProperty(int startIdx, int endIdx)
            :startIdx(startIdx),endIdx(endIdx){}
        const int startIdx, endIdx;
};

class Animator: public FSM<Entity>{
        public:
            void enter(Entity *instance){
                this->timer.start();
            }
            FSM<Entity>* update(Entity *instance, float dt){
                if(!play) return nullptr;
                if(timer.getTicks() >= this->animTick){
                    instance->chooseCurrentSprite(anim);
                    this->anim++;
                    if(this->anim >= currentAnimation->endIdx) 
                        this->anim = currentAnimation->startIdx;
                    this->timer.restart();
                }
                return nullptr;
            }
            void exit(Entity *instance){}

            void addAnimation(std::string name, int num){
                auto anime = new AnimeProperty(last, last+num);
                last += num;
                properties[name] = anime;
            }

            void playAnimation(std::string name){
                auto tmp = properties.find(name);
                if(tmp == properties.end()){
                    printf("Animation '%s' is not found\n",name);
                    return;
                }
                currentAnimation = properties[name];
                this->anim = currentAnimation->startIdx;
                play = true;
            }
            void stopAnimation(){
                play = false;
            }

            void setAnimTick(int tick){
                this->animTick = tick;
            }
            AnimeProperty* getCurrent(){
                return currentAnimation;
            }
            int getCurrentAnim(){
                if(!currentAnimation) return -1;
                return anim - currentAnimation->startIdx;
            }

        private:
            Timer timer;
            int animTick = Timer::TICK_12FRAMES;
            std::unordered_map<std::string, AnimeProperty*> properties;
            AnimeProperty* currentAnimation;
            bool play = false;
            int anim;
            int last = 0;
    };

#endif
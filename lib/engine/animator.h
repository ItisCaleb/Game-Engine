#ifndef ANIMATOR_H_
#define ANIMATOR_H_


#include "fsm.h"

#include <unordered_map>

#include "engine/entity.h"
#include "engine/timer.h"
#include "engine/resource_manager.h"

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

class Animator{
        public:
            void play(Entity *instance, float dt){
                if(stop) return;
                if(timer.getTicks() >= this->animTick){
                    lastAnim = this->anim;
                    instance->chooseCurrentSprite(anim);
                    this->anim++;
                    if(this->anim >= currentAnimation->endIdx) 
                        this->anim = currentAnimation->startIdx;
                    this->timer.restart();
                }
            }

            void addAnimation(std::string name, int num){
                auto anime = new AnimeProperty(last, last+num);
                last += num;
                properties[name] = anime;
            }

            void setAnimation(std::string name){
                auto tmp = properties.find(name);
                if(tmp == properties.end()){
                    printf("Animation '%s' is not found\n",name.c_str());
                    return;
                }
                currentAnimation = properties[name];
                this->anim = currentAnimation->startIdx;
                this->lastAnim = currentAnimation->startIdx;
                stop = false;
            }
            void stopAnimation(){
                stop = true;
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

            bool isEnding(){
                return anim < lastAnim;
            }
            void loadAnimations(std::string path, Entity *instance){
                auto json = ResourceManager::load<nlohmann::json>(path);
                if(!json){
                    printf("Resource '%s' does not exist\n",path.c_str());
                    return;
                }
                for (auto& ani : (*json)["animations"]){
                    int r = ResourceManager::loadSprites(ani["path"],
                        ani["clipW"], ani["clipH"], ani["paddingX"], ani["paddingY"], instance->getSprites());
                    this->addAnimation(ani["name"], r);
                }
                ResourceManager::destroy(json);
            }

        private:
            Timer timer;

            // speed
            int animTick = Timer::TICK_12FRAMES;
            std::unordered_map<std::string, AnimeProperty*> properties;
            AnimeProperty* currentAnimation;
            bool stop = false;
            int anim;
            int lastAnim;
            int last = 0;
    };

#endif
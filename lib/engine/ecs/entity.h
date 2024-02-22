#ifndef ECS_ENTITY_H_
#define ECS_ENTITY_H_

#include "define.h"
#include <queue>
#include <array>

class EntityManager{
    public:
        EntityManager(){
            for(Entity i=0;i<MAX_ENTITIES;i++){
                available.push(i);
            }
        }
        Entity createEntity(){
            Entity id = available.front();
            available.pop();
            entityCount++;
            return id;
        }
        void destroyEntity(Entity e){
            available.push(e);
            entityCount--;
        }
        void setComponentFlag(Entity e, ComponentFlag flag){
            flags[e] = flag;
        }
        ComponentFlag getComponentFlag(Entity e){
            return flags[e];
        }
    private:
        std::queue<Entity> available;
        std::array<ComponentFlag, MAX_ENTITIES> flags;
        std::size_t entityCount = 0;
        
};

#endif
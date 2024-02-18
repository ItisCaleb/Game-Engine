#ifndef ECS_ENTITY_H_
#define ECS_ENTITY_H_

#include <queue>

using Entity = std::size_t;

const Entity MAX_ENTITY = 50000;

class EntityManager{
    public:
        EntityManager(){
            for(Entity i=0;i<MAX_ENTITY;i++){
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
    private:
        std::queue<Entity> available;
        std::size_t entityCount = 0;
};

#endif
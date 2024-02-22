#ifndef ECS_CONTROLLER_H_
#define ECS_CONTROLLER_H_

#include "entity.h"
#include "component.h"
#include "system.h"

class ECS{
    public:
        Entity createEntity(){
            return entManager.createEntity();
        }
        
        void destroyEntity(Entity entity){
            entManager.destroyEntity(entity);
            compManager.entityDestroyed(entity);
            sysManager.entityDestroyed(entity);
        }

        template <class T>
        void registerComponent(){
            compManager.registerComponent<T>();
        }

        template <class T>
        void addComponent(Entity entity, T component){
            compManager.addComponent<T>(entity, component);
            auto flags = entManager.getComponentFlag(entity);
            flags.set(compManager.getComponentType<T>(), true);
            entManager.setComponentFlag(entity, flags);
            sysManager.entityFlagChanged(entity, flags);
        }

        template <class T>
        void removeComponent(Entity entity){
            compManager.removeComponent<T>(entity);
            auto flags = entManager.getComponentFlag(entity);
            flags.set(compManager.getComponentType<T>(), false);
            entManager.setComponentFlag(entity, flags);
            sysManager.entityFlagChanged(entity, flags);
        }

        template <class T>
        T& query(Entity entity){
            return compManager.getComponent<T>(entity);
        }

        template <class T>
        ComponentType queryType(){
            return compManager.getComponentType<T>();
        }


    private:
        EntityManager entManager;
        ComponentManager compManager;
        SystemManager sysManager;
};



#endif
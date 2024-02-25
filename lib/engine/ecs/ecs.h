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

        template <class T>
        T* registerSystem(bool update){
            return sysManager.registerSystem<T>(update);
        }

        template <class T>
        void setSystemFlag(ComponentFlag flag){
            return sysManager.setComponentFlag<T>(flag);
        }

        void update(float dt){
            for (auto &system: sysManager.getSystem()){
                system->update(dt);
            }
        }


    private:
        EntityManager entManager;
        ComponentManager compManager;
        SystemManager sysManager;
};



#endif
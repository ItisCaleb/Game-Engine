#ifndef ECS_COMPONENT_H_
#define ECS_COMPONENT_H_


#include "define.h"
#include <vector>
#include <unordered_map>
#include <assert.h>

class Component{
};

class IComponentArray{
    public:
	    virtual ~IComponentArray() = default;
	    virtual void entityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	void insertData(Entity entity, T component)
	{
		assert(entityToIndex.find(entity) == entityToIndex.end() && "Component added to same entity more than once.");

		// Put new entry at end and update the maps
		size_t newIndex = mSize;
		entityToIndex[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;
		mComponentArray[newIndex] = component;
		++mSize;
	}

	void removeData(Entity entity)
	{
		assert(entityToIndex.find(entity) != entityToIndex.end() && "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = entityToIndex[entity];
		size_t indexOfLastElement = mSize - 1;
		mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

		// Update map to point to moved spot
		Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
		entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		entityToIndex.erase(entity);
		mIndexToEntityMap.erase(indexOfLastElement);

		--mSize;
	}

	T& getData(Entity entity)
	{
		assert(entityToIndex.find(entity) != entityToIndex.end() && "Retrieving non-existent component.");

		// Return a reference to the entity's component
		return mComponentArray[entityToIndex[entity]];
	}

	void entityDestroyed(Entity entity) override
	{
		if (entityToIndex.find(entity) != entityToIndex.end())
		{
			// Remove the entity's component if it existed
			removeData(entity);
		}
	}

private:
	// The packed array of components (of generic type T),
	// set to a specified maximum amount, matching the maximum number
	// of entities allowed to exist simultaneously, so that each entity
	// has a unique spot.
	std::array<T, MAX_ENTITIES> mComponentArray;

	// Map from an entity ID to an array index.
	std::unordered_map<Entity, size_t> entityToIndex;

	// Map from an array index to an entity ID.
	std::unordered_map<size_t, Entity> mIndexToEntityMap;

	// Total size of valid entries in the array.
	size_t mSize;
};

class ComponentManager{
    public:
        template <class T>
        void registerComponent(){
            static_assert(std::is_base_of<Component, T>::value, "This isn't a component");
            auto type_name = typeid(T).name();
			assert(componentTypes.find(type_name) == componentTypes.end());
			componentTypes.insert({type_name, nextType});
            components.insert({type_name, new ComponentArray<T>()});
			nextType++;
        }
		template <class T>
		ComponentType getComponentType(){
            static_assert(std::is_base_of<Component, T>::value, "This isn't a component");
            auto type_name = typeid(T).name();
            assert(componentTypes.find(type_name) != componentTypes.end());
			return componentTypes[type_name];
        }

		template <class T>
		void addComponent(Entity entity, T component){
            static_assert(std::is_base_of<Component, T>::value, "This isn't a component");
            this->getComponentArray<T>()->insertData(entity, component);
			
        }

		template <class T>
		void removeComponent(Entity entity){
            static_assert(std::is_base_of<Component, T>::value, "This isn't a component");
            this->getComponentArray<T>()->removeData(entity);
        }

		template<typename T>
		T& getComponent(Entity entity)
		{
			static_assert(std::is_base_of<Component, T>::value, "This isn't a component");
			// Get a reference to a component from the array for an entity
			return this->getComponentArray<T>()->getData(entity);
		}

		void entityDestroyed(Entity entity){
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it
			for (auto const& pair : components){
				auto const& component = pair.second;

				component->entityDestroyed(entity);
			}
		}

    private:
	    std::unordered_map<const char*, ComponentType> componentTypes;;
        std::unordered_map<const char*, IComponentArray*> components;
		ComponentType nextType = 0;

		template<typename T>
		ComponentArray<T>* getComponentArray(){
			const char* typeName = typeid(T).name();
			assert(componentTypes.find(typeName) != componentTypes.end());

			return static_cast<ComponentArray<T>*>(components[typeName]);
		}
};

#endif
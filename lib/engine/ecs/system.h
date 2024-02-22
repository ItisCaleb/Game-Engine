#ifndef ECS_SYSTEM_H_
#define ECS_SYSTEM_H_

#include "define.h"

#include <set>
#include <unordered_map>

class System{
    public:
        std::set<Entity> entities;
};

class SystemManager
{
public:
	template<typename T>
	T* registerSystem()
	{
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) == systems.end() && "Registering system more than once.");

		// Create a pointer to the system and return it so it can be used externally
		systems.insert({typeName, new T()});
		return system;
	}

	template<typename T>
	void setComponentFlag(ComponentFlag flag)
	{
		const char* typeName = typeid(T).name();

		assert(systems.find(typeName) != systems.end() && "System used before registered.");

		// Set the signature for this system
		flags.insert({typeName, flag});
	}

	void entityDestroyed(Entity entity)
	{
		// Erase a destroyed entity from all system lists
		// mEntities is a set so no check needed
		for (auto const& pair : systems)
		{
			auto const& system = pair.second;

			system->entities.erase(entity);
		}
	}

	void entityFlagChanged(Entity entity, ComponentFlag entityFlag)
	{
		// Notify each system that an entity's signature changed
		for (auto const& pair : systems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemFlag = flags[type];

			// Entity signature matches system signature - insert into set
			if ((entityFlag & systemFlag) == systemFlag)
			{
				system->entities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->entities.erase(entity);
			}
		}
	}

private:
	// Map from system type string pointer to a signature
	std::unordered_map<const char*, ComponentFlag> flags{};

	// Map from system type string pointer to a system pointer
	std::unordered_map<const char*, System*> systems{};
};

#endif
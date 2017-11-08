#pragma once
#include "Engine/Base/Common/Common.h"

#include <map>

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Memory/ComponentPool.h"
#include "Engine/Base/Managers/SystemAdmin.h"

class EntityAdmin : public NonCopyable
{
public:
	EntityAdmin();
	~EntityAdmin();

	uint32_t CreateEntity();

	template <typename T>
	T* GetComponent(const Entity e)
	{
		return(GetComponent<T>(e.GetEntityId()));
	}

	template <typename T>
	T* GetComponent(const Entity* const e)
	{
		return(GetComponent<T>(e->GetEntityId()));
	}

	inline const std::vector<Entity>& GetEntities() { return(mEntities); }

private:
	template <typename T>
	T* AddComponent(uint32_t entityID)
	{
		const ComponentFlag flag = ComponentGroup<T>();
		T* component(mComponentPools[entityID]->GetComponent(entityID));
		return component;
	}

	template <typename T>
	T* GetComponent(uint32_t entityID)
	{
		const ComponentFlag flag = ComponentGroup<T>();
		void* componentPtr = mComponentPools[static_cast<int>(flag)]->GetComponent(entityID);
		T* component = static_cast<T*>(componentPtr);
		return(component);
	}

	void RegisterEntity(Entity* entity);

	std::vector<ComponentPoolBase*> mComponentPools;
	std::vector<Entity> mEntities;
	std::map<uint32_t, Entity*> mEntityMap;
};

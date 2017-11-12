#pragma once
#include "Engine/Base/Common/Common.h"

#include <map>

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Memory/ComponentPool.h"
#include "Engine/Base/Managers/SystemAdmin.h"
#include "Engine/Base/Memory/ComponentPool.h"

class EntityAdmin : public NonCopyable
{
public:
	EntityAdmin();
	~EntityAdmin();

	Entity CreateEntity();

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

	template <typename T>
	T* AddComponent(Entity entity)
	{
		const ComponentFlag flag = ComponentGroup<T>();
		const uint32_t eid = entity.mEntityID;

		if(mComponentPools.size() < flag || !mComponentPools[static_cast<int>(flag)])
		{
			ExpandPoolList<T>();
		}

		void* componentMem = mComponentPools[static_cast<int>(flag)]->AllocComponent(eid);
		T* component = new (componentMem) T();
		return(component);
	}

	// Todo move more initialization stuff into here
	Ptr<ComponentBase> AddComponent(ComponentFlag flag, size_t componentSize, Entity entity)
	{
		const uint32_t eid = entity.mEntityID;

		if(mComponentPools.size() < flag + 1 || !mComponentPools[static_cast<int>(flag)])
		{
			ExpandPoolList(flag, componentSize);
		}

		void* componentMem = mComponentPools[static_cast<int>(flag)]->AllocComponent(eid);
		return(static_cast<ComponentBase*>(componentMem));
	}

	inline const std::vector<Entity>& GetEntities() { return(mEntities); }

private:
	template <typename T>
	T* GetComponent(uint32_t entityID)
	{
		const ComponentFlag flag = ComponentGroup<T>();
		void* componentPtr = mComponentPools[static_cast<int>(flag)]->GetComponent(entityID);
		T* component = static_cast<T*>(componentPtr);
		return(component);
	}

	// We do lazy intialization on component pools
	template <typename T>
	void ExpandPoolList()
	{
		const ComponentFlag flag = ComponentGroup<T>();
		ExpandPoolList(flag, sizeof(T));
	}

	void ExpandPoolList(ComponentFlag flag, size_t blockSize)
	{
		if(mComponentPools.size() < static_cast<int>(flag) + 1)
		{
			mComponentPools.resize(static_cast<size_t>(flag + 1));
		}
		mComponentPools[static_cast<size_t>(flag)] = new ComponentPool(blockSize);
	}

	void RegisterEntity(Entity* entity);

	std::vector<Ptr<ComponentPoolBase>> mComponentPools;
	std::vector<Entity> mEntities;
	std::map<uint32_t, Entity*> mEntityMap;
};

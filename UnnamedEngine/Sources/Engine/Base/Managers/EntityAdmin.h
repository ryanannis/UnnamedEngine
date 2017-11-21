#pragma once
#include "Engine/Base/Common/Common.h"

#include <map>

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Managers/SystemAdmin.h"
#include "Engine/Base/Memory/HashMapPool.h"
#include "Engine/Base/Memory/ArrayPool.h"

class EntityAdmin : public NonCopyable
{
public:
	EntityAdmin();
	~EntityAdmin();

	Entity CreateEntity();

	template <typename T>
	inline T* GetComponent(const Entity& e)
	{
		return(GetComponent<T>(e.GetIndex()));
	}

	template <typename T>
	inline T* AddComponent(Entity entity)
	{
		const ComponentFlag flag = T::GetGroup();
		const uint32_t eid = entity.GetIndex();

		if (mComponentPools.size() <= flag || !mComponentPools[static_cast<int>(flag)])
		{
			ExpandPoolList<T>();
		}

		void* componentMem = mComponentPools[static_cast<int>(flag)]->AllocComponent(eid);
		T* component = new (componentMem) T();
		return(component);
	}

	// todo:  could avoid avoid passing StorageStrategy every time by doing pool
	// allocation on dependency loading...
	Ptr<ComponentBase> AddComponent(ComponentFlag flag, StorageStrategy s, size_t componentSize, Entity entity)
	{
		assert(s != StorageStrategy::Singleton);
		const uint32_t eid = entity.GetIndex();

		if (mComponentPools.size() <= flag || !mComponentPools[static_cast<int>(flag)])
		{
			ExpandPoolList(flag, s, componentSize);
		}

		void* componentMem = mComponentPools[static_cast<int>(flag)]->AllocComponent(eid);
		return(static_cast<ComponentBase*>(componentMem));
	}

	template <typename T>
	Ptr<T> GetSingletonComponent()
	{
		const ComponentFlag flag = T::GetGroup();
		void* componentPtr = mSingletonComponents[static_cast<int>(flag)];
		T* component = static_cast<T*>(componentPtr);
		return(component);
	}

	inline const std::vector<Entity>& GetEntities() { return(mEntities); }

private:
	template <typename T>
	inline T* GetComponent(uint32_t entityID)
	{
		const ComponentFlag flag = T::GetGroup();
		void* componentPtr = mComponentPools[static_cast<int>(flag)]->GetComponent(entityID);
		T* component = static_cast<T*>(componentPtr);
		return(component);
	}

	// We do lazy intialization on component pools
	template <typename T>
	void ExpandPoolList()
	{
		const ComponentFlag flag = T::GetGroup();
		ExpandPoolList(flag, T::GetStorageStrategy(), sizeof(T));
	}

	void ExpandPoolList(ComponentFlag flag, StorageStrategy s, size_t blockSize)
	{
		if (mComponentPools.size() < static_cast<size_t>(flag) + 1)
		{
			mComponentPools.resize(static_cast<size_t>(flag + 1));
		}
		if (s == StorageStrategy::HashMap)
		{
			mComponentPools[static_cast<size_t>(flag)] = new HashMapPool(blockSize);
		}
		else if (s == StorageStrategy::Consecutive)
		{
			mComponentPools[static_cast<size_t>(flag)] = new ArrayPool(blockSize);
		}
	}

	std::vector<Ptr<ComponentPoolBase>> mComponentPools;
	std::vector<void*> mSingletonComponents;

	std::vector<Entity> mDeletedEntities;

	std::vector<size_t> mGenerationTable;

	size_t mEntityCounter;
};

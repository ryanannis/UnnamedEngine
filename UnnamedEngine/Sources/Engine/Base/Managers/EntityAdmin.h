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

	// todo:  replace with iterator
	template <typename T>
	inline std::vector<Entity> GetEntities()
	{
		// TODO:  TOP TIER PRIORITY REPLACE THIS THIS IS VERY BAD~~
		// CONSTRUCT ENTIES FROM KEYS IN POOLS
		std::vector<Entity> matchingEntities;
		for(Entity& e : mEntities)
		{
			if(GetComponent<T>(e))
			{
				matchingEntities.push_back(e);
			}
		}

		return(matchingEntities);
	}

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

		if(mComponentPools.size() <= flag || !mComponentPools[static_cast<int>(flag)])
		{
			ExpandPoolList(flag, s, componentSize);
		}

		void* componentMem = mComponentPools[static_cast<int>(flag)]->AllocComponent(eid);
		return(static_cast<ComponentBase*>(componentMem));
	}

	template <typename T>
	Ptr<T> AddSingletonComponent()
	{
		const ComponentFlag flag = T::GetGroup();
		
		if(mSingletonComponents.size() <= flag || !mSingletonComponents[static_cast<int>(flag)])
		{
			if(mSingletonComponents.size() < static_cast<size_t>(flag) + 1)
			{
				mSingletonComponents.resize(static_cast<size_t>(flag + 1));
			}

			// Ptr<void> is default null
			assert(!mSingletonComponents[flag]);

			mSingletonComponents[flag] = new T;
		}

		void* componentPtr = mSingletonComponents[static_cast<int>(flag)];
		T* component = static_cast<T*>(componentPtr);
		return(component);
	}

	template <typename T>
	Ptr<T> GetSingletonComponent()
	{
		const ComponentFlag flag = T::GetGroup();
		void* componentPtr = mSingletonComponents[static_cast<int>(flag)];
		T* component = static_cast<T*>(componentPtr);
		return(component);
	}

private:
	template <typename T>
	inline T* GetComponent(uint32_t entityID)
	{
		const ComponentFlag flag = T::GetGroup();
		// todo:  ahhh the useless branch!
		if(static_cast<int>(flag) >= mComponentPools.size() || !mComponentPools[static_cast<int>(flag)])
		{
			return(nullptr);
		}
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
		if(mComponentPools.size() < static_cast<size_t>(flag) + 1)
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
	std::vector<Ptr<void>> mSingletonComponents;

	std::vector<Entity> mDeletedEntities;
	std::vector<Entity> mEntities;

	std::vector<size_t> mGenerationTable;

	size_t mEntityCounter;
};

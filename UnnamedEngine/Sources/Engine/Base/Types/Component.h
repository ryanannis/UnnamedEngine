#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/ComponentBase.h"

typedef uint32_t ComponentFlag;

// Need a static but don't want it in a templated class...
struct ComponentStatics
{
	static ComponentFlag SingletonGroupCounter;
	static ComponentFlag StaticGroupCounter;
};

template <typename Derived>
class Component : public ComponentBase {
public:
	Component() {};

	static void RegisterGroup()
	{
		static bool initialized = false;
		assert(!initialized);
		if(!initialized)
		{
			initialized = true;

			// Singleton components are stored in a seperate array in the EntityAdmin
			if(sStorageStrategy == StorageStrategy::Singleton)
			{
				sComponentGroup = ComponentStatics::SingletonGroupCounter++;
			}
			else
			{
				sComponentGroup = ComponentStatics::StaticGroupCounter++;
			}
				
		}
	}

	FORCEINLINE static const ComponentFlag GetGroup()
	{
		return(sComponentGroup);
	}

	virtual std::string GetName() const
	{
		return(sName);
	}

	static const StorageStrategy GetStorageStrategy()
	{
		return(sStorageStrategy);
	}

	static ComponentFlag sComponentGroup;

protected:
	static bool sSerializable;
	static std::string sName;
	static StorageStrategy sStorageStrategy;
};

// Get the group from just the type
template <typename T>
static ComponentFlag ComponentGroup() {
	return Component<typename std::remove_const<T>::type>::GetGroup();
}

template <typename T> ComponentFlag Component<T>::sComponentGroup = 0;
#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/ComponentBase.h"

typedef uint64_t ComponentFlag;
static ComponentFlag StaticGroupCounter = 0;

template <typename Derived>
class Component : public ComponentBase {
public:
	Component() {};

	static ComponentFlag GetGroup()
	{
		static bool initialized = false;
		if(initialized)
		{
			initialized = true;
			sComponentGroup = StaticGroupCounter++;
		}
		return(sComponentGroup);
	}

	virtual std::string GetName()
	{
		return(sName);
	}

	static StorageStrategy GetStorageStrategy()
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
#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/ComponentBase.h"

typedef uint64_t ComponentFlag;
static ComponentFlag sComponentGroup = 0;

template <typename Derived>
class Component : public ComponentBase {
public:
	Component() {};

	static ComponentFlag GetGroup()
	{
		static const ComponentFlag group = sComponentGroup++;
		return(group);
	}

	virtual std::string GetName()
	{
		return(sName);
	}

	static StorageStrategy GetStorageStrategy()
	{
		return(sStorageStrategy);
	}

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

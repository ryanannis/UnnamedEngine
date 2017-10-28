#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/ComponentBase.h"

typedef uint64_t ComponentFlag;
static ComponentFlag sComponentGroup = 0;

template <typename Derived>
class Component : public ComponentBase {
public:
	Component() {};
	virtual ~Component() = 0;
	virtual uint64_t GetComponentID() = 0;

	static ComponentFlag GetGroup()
	{
		static const int group = sComponentGroup++;
		return(group);
	}
};
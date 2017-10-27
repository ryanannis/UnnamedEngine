#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"

typedef uint64_t ComponentFlag;
static ComponentFlag sComponentGroup = 0;

class Component{
public:
	Component();
	virtual ~Component() = 0;
	virtual uint64_t GetComponentID() = 0;

	static inline const ComponentFlag GetGroup()
	{
		static const int group = sComponentGroup++;
		return(group);
	}
};
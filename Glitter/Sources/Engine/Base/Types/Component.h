#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"

static uint32_t componentGroup = 0;

class Component{
public:
	Component();
	virtual ~Component() = 0;
	virtual uint64_t GetComponentID() = 0;

	static inline const uint32_t GetGroup()
	{
		static const int group = componentGroup++;
		return(group);
	}
};
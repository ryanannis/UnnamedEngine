#pragma once

#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/Entity.h"

static uint32_t systemGroup = 0;

class System : public NonCopyable {
public:
	virtual void RegisterEntity(const Entity& e) = 0 ;
	virtual void Update(const float delta) = 0;
	
	static inline const uint32_t GetGroup()
	{
		static const int group = systemGroup++;
		return(group);
	}
};
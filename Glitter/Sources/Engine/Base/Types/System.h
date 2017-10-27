#pragma once

#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/Entity.h"

typedef uint64_t SystemFlag;
static SystemFlag sSystemGroup = 0;

class System : public NonCopyable {
public:
	virtual void RegisterEntity(const Entity& e) = 0 ;
	virtual void Update(const float delta) = 0;
	
	static inline const SystemFlag GetGroup()
	{
		static const int group = sSystemGroup++;
		return(group);
	}
};
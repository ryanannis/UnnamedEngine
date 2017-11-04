#pragma once

#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Types/SystemBase.h"

class SystemAdmin;

typedef uint64_t SystemFlag;
static SystemFlag sSystemGroup = 0;

template <typename Derived>
class System : public SystemBase {
public:

private:
	static SystemFlag SystemGroup()
	{
		static const int group = sSystemGroup++;
		return(group);
	}

	friend SystemAdmin;
};
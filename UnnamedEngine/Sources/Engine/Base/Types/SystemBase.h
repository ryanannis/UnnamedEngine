#pragma once
#include "Engine/Base/Common/Common.h"

class EntitySystem;

class SystemBase {
public:
	virtual void Update(float dt, Ptr<EntityAdmin> entityAdmin) = 0;
};
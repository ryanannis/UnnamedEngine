#pragma once
#include "Engine/Base/Common/Common.h"

class SystemBase {
public:
	virtual void Update(float dt, EntityAdmin* entityAdmin) = 0;
};
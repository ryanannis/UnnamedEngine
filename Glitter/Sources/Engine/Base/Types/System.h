#pragma once

#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/Entity.h"

class System {
public:
	virtual void RegisterEntity(const Entity& e) = 0 ;
	virtual void Update(const float delta) = 0;

private:
	// Disable copying
	System& operator=(const System&) = delete;
	System(const System&) = delete;
};
#pragma once

#include "Engine/Base/Types/System.h"

class PlayerInputSystem : public System<PlayerInputSystem> {
public:
	virtual void Update(float dt, Ptr<EntityAdmin> entityAdmin) override;
	virtual void StaticInitDependencies() override;
};

#pragma once

#include "Engine/Base/Types/System.h"

class Context;

class PlayerInputSystem : public System<PlayerInputSystem> {
public:
	PlayerInputSystem(Ptr<Context> context);

	virtual void Update(float dt, Ptr<EntityAdmin> entityAdmin) override;
	virtual void StaticInitDependencies() override;

private:
	Ptr<Context> mContext;
};

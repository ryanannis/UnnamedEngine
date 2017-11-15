#pragma once

#include "Engine/Base/Types/System.h"

class Context;

class RenderSystem : public System<RenderSystem> {
public:
	RenderSystem(Ptr<Context> mContext);
	virtual void Update(float dt, Ptr<EntityAdmin> entityAdmin) override;
	virtual void StaticInitDependencies() override;

private:
	const Ptr<Context> mContext;
};


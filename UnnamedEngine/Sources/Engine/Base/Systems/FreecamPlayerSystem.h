#pragma once

#include "Engine/Base/Types/System.h"

class Context;

class FreecamPlayerSystem : public System<FreecamPlayerSystem> {
public:
	FreecamPlayerSystem(Ptr<Context> context);

	virtual void Update(float dt, Ptr<EntityAdmin> entityAdmin) override;
	virtual void StaticInitDependencies() override;

private:
	Ptr<Context> mContext;
};

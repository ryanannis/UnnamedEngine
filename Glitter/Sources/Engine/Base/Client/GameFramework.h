#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Managers/RegionAdmin.h"

class Context;

class GameFramework
{
public:
	explicit GameFramework(Ptr<Context> context);
	void Update(float dt);

private:
	Ptr<Context> mContext;
	Ptr<RegionAdmin> mRegionAdmin;
};
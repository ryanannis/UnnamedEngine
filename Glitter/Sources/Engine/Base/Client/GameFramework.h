#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Managers/RegionAdmin.h"

class Context;

class GameFramework
{
public:
	explicit GameFramework(Ptr<Context> context);
	virtual ~GameFramework();
	void Update(float dt);
	void Ready();

private:

	Ptr<Context> mContext;
	std::unique_ptr<RegionAdmin> mRegionAdmin;
};
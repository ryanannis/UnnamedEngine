#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Managers/RegionAdmin.h"
#include "Engine/Base/Managers/SystemAdmin.h"

class Context;

class GameFramework
{
public:
	explicit GameFramework(Ptr<Context> context);
	void Update(float dt);
	Ptr<SystemAdmin> GetSystemAdmin() { return(&mSystemAdmin); }

	void Initialize();

private:

	void InitSystems();
	Ptr<Context> mContext;
	std::unique_ptr<RegionAdmin> mRegionAdmin;
	
	SystemAdmin mSystemAdmin;
};
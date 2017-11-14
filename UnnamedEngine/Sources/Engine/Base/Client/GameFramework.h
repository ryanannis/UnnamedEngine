#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Managers/SystemAdmin.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Managers/RegionAdmin.h"

class Context;

class GameFramework
{
public:
	explicit GameFramework(Ptr<Context> context);
	void Update(float dt);

	void Initialize();
	void InitSystems();
	Ptr<ResourceManager> GetResourceManager();

private:

	Ptr<Context> mContext;
	std::unique_ptr<RegionAdmin> mRegionAdmin;
	std::unique_ptr<ResourceManager> mResourceManager;
	SystemAdmin mSystemAdmin;
};
#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Managers/SystemAdmin.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Level/LevelResource.h"
#include "Engine/Base/Managers/RegionAdmin.h"

class Context;
class RegionAdmin;

class GameFramework
{
public:
	explicit GameFramework(Ptr<Context> context);
	void Update(float dt);

	void Initialize();
	void InitSystems();
	void LoadIntoLevel(ResourceType<LevelResource> levelResource);

	Ptr<ResourceManager> GetResourceManager();

private:

	Ptr<Context> mContext;
	std::unique_ptr<RegionAdmin> mRegionAdmin;
	std::unique_ptr<ResourceManager> mResourceManager;
	SystemAdmin mSystemAdmin;
};
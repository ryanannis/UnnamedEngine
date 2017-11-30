#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Managers/SystemAdmin.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Level/LevelResource.h"
#include "Engine/Base/Managers/RegionAdmin.h"
#include "Engine/Base/Client/GameClient.h"

class Context;
class RegionAdmin;
class Renderer;

class GameFramework
{
public:
	explicit GameFramework(Ptr<Context> context);
	void Update(float dt);

	void Initialize();
	void InitSystems();
	void InitSingletons();

	void LoadIntoLevel(ResourceType<LevelResource> levelResource);

	Ptr<ResourceManager> GetResourceManager();
	Ptr<RegionAdmin> GetRegionAdmin();
	Ptr<GameClient> GetGameClient();
private:

	Ptr<Context> mContext;
	std::unique_ptr<GameClient> mGameClient;
	std::unique_ptr<RegionAdmin> mRegionAdmin;
	std::unique_ptr<ResourceManager> mResourceManager;
	SystemAdmin mSystemAdmin;
};
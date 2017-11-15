#include "GameFramework.h"
#include <memory>

// Systems
#include "Engine/Base/Systems/RenderSystem.h"
#include "Engine/Base/Systems/PlayerInputSystem.h"

GameFramework::GameFramework(Ptr<Context> context):
	mContext(context)
{
}

void GameFramework::Update(float dt)
{
	if(mRegionAdmin)
	{
		auto entityAdmin = mRegionAdmin->GetEntityAdmin();
		mSystemAdmin.Update(dt, entityAdmin);
	}
}

Ptr<ResourceManager> GameFramework::GetResourceManager()
{
	return(mResourceManager.get());
}

void GameFramework::Initialize()
{
	mResourceManager = std::make_unique<ResourceManager>(mContext);
}

void GameFramework::InitSystems()
{
	mSystemAdmin.AddSystem<PlayerInputSystem>();
	mSystemAdmin.AddSystem<RenderSystem>();
}

void GameFramework::LoadIntoLevel(ResourceType<LevelResource> levelResource)
{
	assert(!mRegionAdmin);
	mRegionAdmin = std::make_unique<RegionAdmin>(mContext);
	std::shared_ptr<LevelResource> level = mResourceManager->LoadResource(levelResource).lock();
	mRegionAdmin->LoadLevel(level);
	// todo:  this is running sync - will get a MASSIVE hitch 
}

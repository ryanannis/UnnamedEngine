#include "GameFramework.h"
#include <memory>

// Systems
#include "Engine/Base/Systems/RenderSystem.h"
#include "Engine/Base/Systems/PlayerInputSystem.h"
#include "Engine/Graphics/Renderer/Renderer.h"
#include "Engine/Base/Client/Context.h"

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

void GameFramework::InitializeRenderer()
{
	RenderSettings s;
	// hardcoded for now
	s.screenHeight = 720;
	s.screenWidth = 1280;
	mRenderer = std::make_unique<Renderer>();
}

Ptr<ResourceManager> GameFramework::GetResourceManager()
{
	return(mResourceManager.get());
}

Ptr<Renderer> GameFramework::GetRenderer()
{
	return(mRenderer.get());
}

void GameFramework::Initialize()
{
	mResourceManager = std::make_unique<ResourceManager>(mContext);
}

void GameFramework::InitSystems()
{
	mSystemAdmin.AddSystem<PlayerInputSystem>(mContext);
	mSystemAdmin.AddSystem<RenderSystem>(mContext);
}

void GameFramework::LoadIntoLevel(ResourceType<LevelResource> levelResource)
{
	assert(!mRegionAdmin);
	mRegionAdmin = std::make_unique<RegionAdmin>(mContext);
	std::shared_ptr<LevelResource> level = mResourceManager->LoadResource(levelResource).lock();
	mRegionAdmin->LoadLevel(level);
	// todo:  this is running sync - will get a MASSIVE hitch 
}

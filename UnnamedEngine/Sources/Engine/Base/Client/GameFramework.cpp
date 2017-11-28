#include "GameFramework.h"
#include <memory>

#include "Engine/Base/Client/Context.h"

// Systems
#include "Engine/Base/Systems/RenderSystem.h"
#include "Engine/Base/Systems/PlayerInputSystem.h"
#include "Engine/Graphics/Renderer/Renderer.h"

// Singletons
#include "Engine/Base/Components/SingletonComponents/InputComponent.h"

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

Ptr<RegionAdmin> GameFramework::GetRegionAdmin()
{
	return(mRegionAdmin.get());
}

void GameFramework::Initialize()
{
	mResourceManager = std::make_unique<ResourceManager>(mContext);
	InitSystems();
}

// Essentially a registry for systems
void GameFramework::InitSystems()
{
	mSystemAdmin.AddSystem<PlayerInputSystem>(mContext);
	mSystemAdmin.AddSystem<RenderSystem>(mContext);
}

// Essentially a registry for singletons
void GameFramework::InitSingletons()
{
	mRegionAdmin->GetEntityAdmin()->AddSingletonComponent<InputComponent>();
}

void GameFramework::LoadIntoLevel(ResourceType<LevelResource> levelResource)
{
	assert(!mRegionAdmin);
	mRegionAdmin = std::make_unique<RegionAdmin>(mContext);
	std::shared_ptr<LevelResource> level = mResourceManager->LoadResource(levelResource).lock();
	mRegionAdmin->LoadLevel(level);
	InitSingletons();
	// todo:  this is running sync - will get a MASSIVE hitch 
}

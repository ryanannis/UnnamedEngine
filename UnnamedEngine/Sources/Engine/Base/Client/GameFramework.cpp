#include "GameFramework.h"
#include <memory>

// Systems
#include "Engine/Base/Systems/RenderSystem.h"
#include "Engine/Base/Systems/PlayerInputSystem.h"

GameFramework::GameFramework(Ptr<Context> context):
	mContext(context)
{
}

void GameFramework::Update(float)
{
	if(mRegionAdmin)
	{

	}
}

Ptr<ResourceManager> GameFramework::GetResourceManager()
{
	return(mResourceManager.get());
}

void GameFramework::Initialize()
{
	mRegionAdmin = std::make_unique<RegionAdmin>(mContext);
	mResourceManager = std::make_unique<ResourceManager>(mContext);
}

void GameFramework::InitSystems()
{
	mSystemAdmin.AddSystem<PlayerInputSystem>();
	mSystemAdmin.AddSystem<RenderSystem>();
}
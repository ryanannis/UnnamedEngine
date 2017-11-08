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

void GameFramework::Initialize()
{
	mRegionAdmin = std::make_unique<RegionAdmin>(mContext);
	mResourceManager = std::make_unique<ResourceManager>(mResourceManager);
}

void GameFramework::InitSystems()
{
	mSystemAdmin.AddSystem<PlayerInputSystem>();
	mSystemAdmin.AddSystem<RenderSystem>();
}
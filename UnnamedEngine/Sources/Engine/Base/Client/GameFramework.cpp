#include "GameFramework.h"
#include <memory>

GameFramework::GameFramework(Ptr<Context> context):
	mContext(context)
{
}

void GameFramework::Update(float dt)
{
	if(mRegionAdmin)
	{

	}
}

void GameFramework::Initialize()
{
	mRegionAdmin = std::make_unique<RegionAdmin>(mContext);
	InitSystems();
}


void GameFramework::InitSystems()
{
	
}
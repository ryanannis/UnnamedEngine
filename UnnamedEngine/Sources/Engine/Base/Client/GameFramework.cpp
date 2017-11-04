#include "GameFramework.h"
#include <memory>

GameFramework::GameFramework(Ptr<Context> context):
	mContext(context)
{
	Ready();
}

void GameFramework::Update(float dt)
{
	if(mRegionAdmin)
	{
		
	}
}

void GameFramework::Ready()
{
	mRegionAdmin = std::make_unique<RegionAdmin>(mContext);
}


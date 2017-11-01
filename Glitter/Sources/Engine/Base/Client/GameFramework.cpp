#include "GameFramework.h"

GameFramework::GameFramework(Ptr<Context> context):
	mContext(context),
	mRegionAdmin(context)
{
}

void GameFramework::Update(float dt)
{
	assert(mContext != nullptr); // Did you call SetContext?
}


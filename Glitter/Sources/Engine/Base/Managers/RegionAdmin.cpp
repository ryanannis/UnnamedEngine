#include "RegionAdmin.h"

RegionAdmin::RegionAdmin(Ptr<Context> context) :
	mContext(context),
	mEntityAdmin(),
	mSystemAdmin(&mEntityAdmin)
{
}

void RegionAdmin::Update(float dt)
{
	mSystemAdmin.Update(dt);
}
#include "RegionAdmin.h"

RegionAdmin::RegionAdmin(Ptr<Context> context) :
	mContext(context),
	mEntityAdmin(),
	mSystemAdmin()
{
}

void RegionAdmin::Update(float dt)
{
	mSystemAdmin.Update(dt, &mEntityAdmin);
}
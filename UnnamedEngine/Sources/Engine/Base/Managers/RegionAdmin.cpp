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

// Loads the Entity and returns it.
// The Entity will have all it's components loaded in at the end of the frame.
Entity RegionAdmin::CreateEntity(std::string URI, bool defer)
{
	// todo: start loading the resource if deferred before 
	if(defer)
	{
		Entity deferredEntity = mEntityAdmin.CreateEntity();
		mDeferredEntities.emplace(URI, deferredEntity);
	}
	Entity entity = mEntityAdmin.CreateEntity();
	return(entity);
}

void RegionAdmin::CreateDeferredEntities()
{
}
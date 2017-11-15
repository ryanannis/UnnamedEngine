#include "RegionAdmin.h"
#include "Engine/Base/Level/LevelResource.h"
#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Client/Client.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Components/TransformComponent.h"

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

void RegionAdmin::LoadLevel(const std::shared_ptr<LevelResource>& level)
{
	assert(level->IsReady());
	for(const auto& staticObject : level->GetLevelObjects())
	{
		const auto& res = staticObject.GetResource();
		const auto& pos = staticObject.GetPosition();
		const auto& rot = staticObject.GetEulerRotation();
		CreateEntity(res, pos, rot, false);
	}
}

Entity RegionAdmin::CreateEntity(const ResourceType<EntityResource>& res, Vector3f position, Vector3f rotation, bool defer)
{
	if(defer)
	{
		Entity deferredEntity = mEntityAdmin.CreateEntity();
		DeferredEntity deferred(res, deferredEntity, position, rotation);
		mDeferredEntities.push(deferred);
		return(deferredEntity);
	}
	else
	{
		auto resourceManager = mContext->GetResourceManager();
		
		// Oh no!  Spot loading is bad!
		std::shared_ptr<EntityResource> loadedRes = resourceManager->LoadResource(res).lock();
		Entity e = loadedRes->ConstructEntity(mEntityAdmin);

		// todo:  DON'T BLOCK TRANSFORM COMPONENT
		auto transformComponent = mEntityAdmin.GetComponent<TransformComponent>(e);
		if(transformComponent)
		{
			// todo:  figure out how to initialize this in a non-shitty way
			transformComponent->pEntityWorldRotation = position;
			transformComponent->pEntityWorldRotation = rotation;
		}
		return(e);
	}
}

void RegionAdmin::CreateDeferredEntities()
{
	while(!mDeferredEntities.empty())
	{
		const auto deferredEntity = mDeferredEntities.front();
		CreateEntity(deferredEntity.resource, deferredEntity.position, deferredEntity.rotation, false);
	}
}
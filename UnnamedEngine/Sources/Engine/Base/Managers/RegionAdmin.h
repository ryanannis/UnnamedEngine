#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Managers/SystemAdmin.h"
#include "Engine/Base/Resource/EntityResource.h"
#include <queue>

class Context;
class LevelResource;

struct DeferredEntity
{
	DeferredEntity(const ResourceType<EntityResource> resource, Entity handle, Vector3f position, Vector3f rotation) :
		resource(resource),
		handle(handle),
		position(position),
		rotation(rotation)
		{}

	const ResourceType<EntityResource> resource;
	const Entity handle;
	Vector3f position;
	Vector3f rotation;
};

class RegionAdmin : public NonCopyable
{
public:
	explicit RegionAdmin(Ptr<Context> context);
	Ptr<const EntityAdmin> GetEntityAdmin() const { return(&mEntityAdmin); };
	void LoadLevel(const LevelResource& level);
	Entity CreateEntity(const ResourceType<EntityResource>&, Vector3f position, Vector3f Rotation, bool defer);
	void Update(float dt);

private:
	void CreateDeferredEntities();

	EntityAdmin mEntityAdmin;
	SystemAdmin mSystemAdmin;
	Ptr<Context> mContext;

	std::queue<DeferredEntity> mDeferredEntities;
};

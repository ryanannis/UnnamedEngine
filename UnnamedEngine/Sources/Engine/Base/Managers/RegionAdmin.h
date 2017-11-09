#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Managers/SystemAdmin.h"
#include <queue>

class Context;

struct DeferredEntity
{
	DeferredEntity(std::string URI, Entity handle) :
		URI(URI),
		handle(handle)
		{}

	const std::string URI;
	const Entity handle;
};

class RegionAdmin : public NonCopyable
{
public:
	explicit RegionAdmin(Ptr<Context> context);
	Ptr<const EntityAdmin> GetEntityAdmin() const { return(&mEntityAdmin); };
	Entity CreateEntity(std::string URI, bool defer);
	void Update(float dt);

private:
	void CreateDeferredEntities();

	EntityAdmin mEntityAdmin;
	SystemAdmin mSystemAdmin;
	Ptr<Context> mContext;

	std::queue<DeferredEntity> mDeferredEntities;
};

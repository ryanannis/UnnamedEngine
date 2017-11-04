#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Managers/SystemAdmin.h"

class Context;

class RegionAdmin : public NonCopyable
{
public:
	explicit RegionAdmin(Ptr<Context> context);
	Ptr<const EntityAdmin> GetEntityAdmin() const { return(&mEntityAdmin); };
	void Update(float dt);

private:
	EntityAdmin mEntityAdmin;
	SystemAdmin mSystemAdmin;
	Ptr<Context> mContext;
};

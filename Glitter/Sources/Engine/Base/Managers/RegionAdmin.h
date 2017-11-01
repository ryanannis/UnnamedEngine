#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Managers/SystemAdmin.h"

class RegionAdmin : public NonCopyable
{
public:
	EntityAdmin* GetEntityAdmin() const {};
	void Update(float dt);

private:
	EntityAdmin mEntityAdmin;
	SystemAdmin mSystemAdmin;

};

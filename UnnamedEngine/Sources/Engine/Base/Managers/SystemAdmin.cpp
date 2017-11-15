#include "SystemAdmin.h"

#include "Engine/Base/Client/Context.h"

SystemAdmin::SystemAdmin()
{
}

void SystemAdmin::Update(float dt, Ptr<EntityAdmin> entityAdmin)
{
	for(auto& system : mSystems)
	{
		system->Update(dt, entityAdmin);
	}
}

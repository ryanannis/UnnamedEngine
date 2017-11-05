#include "SystemAdmin.h"

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

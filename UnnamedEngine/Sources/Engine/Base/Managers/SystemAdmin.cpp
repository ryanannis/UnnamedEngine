#include "SystemAdmin.h"

#include "Engine/Base/Client/Context.h"

SystemAdmin::SystemAdmin()
{
}

void SystemAdmin::Update(float dt, Ptr<EntityAdmin> entityAdmin)
{
#ifdef BUILD_MT
#else
	for(auto& system : mSystems)
	{
		system->Update(dt, entityAdmin);
	}
#endif
}

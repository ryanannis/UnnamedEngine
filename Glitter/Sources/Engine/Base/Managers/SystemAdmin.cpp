#include "SystemAdmin.h"

SystemAdmin::SystemAdmin(EntityAdmin* entityAdmin) :
	mEntityAdmin{ mEntityAdmin }
{
}

void SystemAdmin::Update(float dt)
{
	for(auto& system : mSystems)
	{
		system->Update(dt, mEntityAdmin);
	}
}
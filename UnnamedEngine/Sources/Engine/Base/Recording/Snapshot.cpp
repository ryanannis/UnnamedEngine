#include "Snapshot.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Types/StaticComponent.h"

void Snapshot::Take(const EntityAdmin& e)
{
	const auto& componentRegistry = StaticReg::GetStaticRegistry();
	for(const auto& componentType : componentRegistry)
	{
		
	}
}

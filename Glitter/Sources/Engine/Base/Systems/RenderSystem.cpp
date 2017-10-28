#include "RenderSystem.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Components/RenderComponent.h"

void RenderSystem::Update(float dt, EntityAdmin* entityAdmin)
{
	for(const Entity& e : entityAdmin->GetEntities())
	{
		RenderComponent* renderComponent = entityAdmin->GetComponent<RenderComponent>(e);

	}
}

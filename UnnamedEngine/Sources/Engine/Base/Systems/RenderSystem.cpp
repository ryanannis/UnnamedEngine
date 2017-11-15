#include "RenderSystem.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Components/RenderComponent.h"
#include "Engine/Base/Components/TransformComponent.h"
#include "Engine/Base/Components/CameraComponent.h"
#include "Engine/Base/Client/Context.h"

RenderSystem::RenderSystem(Ptr<Context> context) :
	mContext(context)
{

}

void RenderSystem::Update(float, Ptr<EntityAdmin> entityAdmin)
{
	for(const Entity e : entityAdmin->GetEntities())
	{
		Ptr<RenderComponent> renderComponent = GetComponent<RenderComponent>(entityAdmin, e);
		Ptr<TransformComponent> transformComponent = GetComponent<TransformComponent>(entityAdmin, e);
		
	}
}

void RenderSystem::StaticInitDependencies()
{
	AddReadDependency(ComponentGroup<RenderComponent>());
	AddReadDependency(ComponentGroup<TransformComponent>());
}
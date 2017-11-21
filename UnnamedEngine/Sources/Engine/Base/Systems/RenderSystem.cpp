#include "RenderSystem.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Components/RenderComponent.h"
#include "Engine/Base/Components/TransformComponent.h"
#include "Engine/Base/Components/CameraComponent.h"
#include "Engine/Base/Client/Context.h"
#include "Engine/Graphics/Renderer/Renderer.h"

RenderSystem::RenderSystem(Ptr<Context> context) :
	mContext(context)
{
}

void RenderSystem::Update(float, Ptr<EntityAdmin> entityAdmin)
{
	Ptr<Renderer> renderer = mContext->GetRenderer();
	for(const Entity& e : entityAdmin->GetEntities())
	{
		Ptr<const RenderComponent> renderComponent = GetComponent<const RenderComponent>(entityAdmin, e);
		Ptr<const TransformComponent> transformComponent = GetComponent<const TransformComponent>(entityAdmin, e);
		
	}
}

void RenderSystem::StaticInitDependencies()
{
	AddReadDependency(ComponentGroup<RenderComponent>());
	AddReadDependency(ComponentGroup<TransformComponent>());
}
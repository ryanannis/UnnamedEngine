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
	// Update camera
	const auto& cameraEntities = entityAdmin->GetEntities<CameraComponent>();
	assert(cameraEntities.size() == 1);

	const auto camera = GetComponent<const CameraComponent>(entityAdmin, cameraEntities[0]);
	const auto& rotation = camera->pEntityCameraRotation;
	const auto& translation = camera->pEntityCameraTranslation;

	CameraData c;
	c.fov = 180;
	c.rotation = rotation;
	c.translation = translation;

	mContext->GetRenderer()->SetCameraData(std::move(c));

	// Update entity's graphics data
	Ptr<Renderer> renderer = mContext->GetRenderer();
	for(const Entity& e : entityAdmin->GetEntities<RenderComponent>())
	{
		Ptr<const RenderComponent> renderComponent = GetComponent<const RenderComponent>(entityAdmin, e);
		Ptr<const TransformComponent> transformComponent = GetComponent<const TransformComponent>(entityAdmin, e);
		auto& graphicsData = mContext->GetRenderer()->GetGraphicsData(renderComponent->handle);
		graphicsData.rotation = transformComponent->pEntityWorldRotation;
		graphicsData.translation = transformComponent->pEntityWorldTranslation;
	}
}

void RenderSystem::StaticInitDependencies()
{
	AddReadDependency(ComponentGroup<CameraComponent>());
	AddReadDependency(ComponentGroup<RenderComponent>());
	AddReadDependency(ComponentGroup<TransformComponent>());
}
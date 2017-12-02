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

	const auto cameraEntity = cameraEntities[0];
	const auto camera = GetComponent<const CameraComponent>(entityAdmin, cameraEntity);
	const auto& rotation = camera->pCameraRotation;
	const auto& translation = camera->pEntityCameraTranslation;

	const auto transform = GetComponent<const TransformComponent>(entityAdmin, cameraEntity);

	// todo:  temp
	CameraData c;
	c.fov = 30;
	c.aspectRatio = 1.78f;
	c.rotation = rotation;
	c.translation = translation;

	assert(transform); // i sure hope the camera has a transform...
	c.translation += transform->pEntityWorldTranslation;

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
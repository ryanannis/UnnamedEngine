#include "FreecamPlayerSystem.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Client/ClientInputManager.h"
#include "Engine/Base/Components/SingletonComponents/InputComponent.h"
#include "Engine/Base/Components/TransformComponent.h"
#include "Engine/Base/Components/CameraComponent.h"
#include "Engine/Base/Client/GameFramework.h"
#include "Engine/Base/Client/Keycodes.h"

FreecamPlayerSystem::FreecamPlayerSystem(Ptr<Context> context) :
	mContext(context)
{}

void FreecamPlayerSystem::Update(float delta, Ptr<EntityAdmin> e)
{
	const float SENSITIVITY = 0.001;  // rot = pixels * deg * SENSITIVITY
	const float FREECAMSPEED = 1.0;  // 10m/s
	Ptr<const InputComponent> inputComponent = GetSingletonComponent<InputComponent>(e);
	const Entity& playerEntity = mContext->GetGameFramework()->GetGameClient()->GetLocalPlayerEntity();
	Ptr<TransformComponent> transformComponent = GetWriteComponent<TransformComponent>(e, playerEntity);
	Ptr<CameraComponent> cameraComponent = GetWriteComponent<CameraComponent>(e, playerEntity);

	float mouseDx = 0.0f;
	float mouseDy = 0.0f;
	float forward = 0.0f;
	float right = 0.0f;

	// to make right and left inputs 'cancel'
	size_t horizontalInputs = 0;
	size_t forwardInputs = 0;

	for(auto input : inputComponent->inputEvents)
	{
		if(input.GetKeycode() == KEY_W)
		{
			forward = 1.0;
			forwardInputs++;
		}
		else if(input.GetKeycode() == KEY_S)
		{
			forward = -1.0f;
			forwardInputs++;
		}
		else if(input.GetKeycode() == KEY_A)
		{
			right = -1.0f;
			horizontalInputs++;
		}
		else if(input.GetKeycode() == KEY_D)
		{
			right = 1.0f;
			horizontalInputs++;
		}
		else if(input.GetKeycode() == MOUSE_X)
		{
			mouseDx = input.GetValue();
		}
		else if(input.GetKeycode() == MOUSE_Y)
		{
			mouseDy = input.GetValue();
		}
	}

	if(horizontalInputs > 1)
	{
		right = 0.0f;
	}
	if(forwardInputs > 1)
	{
		forward = 0.0f;
	}

	// Update entity transform
	auto viewRotation = glm::mat3_cast(transformComponent->pEntityWorldRotation * cameraComponent->pEntityCameraRotation);
	transformComponent->pEntityWorldTranslation += viewRotation * Vector3f(right, 0, forward);

	// Update entity camera transform
	auto cameraEulers = glm::eulerAngles(cameraComponent->pEntityCameraRotation);
	cameraEulers.y += mouseDx * SENSITIVITY; // pitch
	cameraEulers.x += mouseDy * SENSITIVITY; // yaw
	cameraComponent->pEntityCameraRotation = glm::quat(cameraEulers);
}

void FreecamPlayerSystem::StaticInitDependencies()
{
	AddWriteDependency(ComponentGroup<CameraComponent>());
	AddReadDependency(ComponentGroup<InputComponent>());
	AddWriteDependency(ComponentGroup<TransformComponent>());
}
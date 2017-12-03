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
	const float SENSITIVITY = 0.001;  // rot = pixels * rad * SENSITIVITY
	const float FREECAMSPEED = 100.0;  // 3m/s
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
		if(input.GetKeycode() == MOUSE_X)
		{
			mouseDx = input.GetValue();
		}
		else if(input.GetKeycode() == MOUSE_Y)
		{
			mouseDy = input.GetValue();
		}
	}

	if(inputComponent->keycodeDown[KEY_W.keycode])
	{
		forward = 1.0;
		forwardInputs++;
	}
	if(inputComponent->keycodeDown[KEY_S.keycode])
	{
		forward = -1.0f;
		forwardInputs++;
	}
	if(inputComponent->keycodeDown[KEY_A.keycode])
	{
		right = -1.0f;
		horizontalInputs++;
	}
	if(inputComponent->keycodeDown[KEY_D.keycode])
	{
		right = 1.0f;
		horizontalInputs++;
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
	auto viewRotation = glm::mat3_cast(transformComponent->pEntityWorldRotation * Quat(cameraComponent->pCameraRotation));
	
	const auto& camRot = cameraComponent->pCameraRotation;
	const Vector3f up(0.0, 1.0, 0.0);
	const Vector3f lookDirection(cos(camRot.y) * cos(camRot.x), sin(camRot.y), cos(camRot.y) * sin(camRot.x));

	transformComponent->pEntityWorldTranslation += glm::normalize(lookDirection) * forward * FREECAMSPEED * delta;
	transformComponent->pEntityWorldTranslation += glm::normalize(glm::cross(lookDirection, up)) * right * FREECAMSPEED * delta;

	auto cameraEulers = cameraComponent->pCameraRotation;

	// Update entity camera transform
	cameraEulers.x += mouseDx * SENSITIVITY; // pitch (counterclockwise for right/pos x)
	cameraEulers.y -= mouseDy * SENSITIVITY; // yaw

	cameraEulers.x += 2 * M_PI;
	cameraEulers.x = fmod(cameraEulers.x, 2 * M_PI);
	
	// can't let pitch get to 0 (cuz then it's the same as the up vector)
	// so do a hard stop
	cameraEulers.y = glm::clamp(
		cameraEulers.y, 
		static_cast<float>(-M_PI / 2 + M_PI / 16), 
		static_cast<float>(M_PI / 2 - M_PI / 16)
	);

	cameraComponent->pCameraRotation = cameraEulers;
}

void FreecamPlayerSystem::StaticInitDependencies()
{
	AddWriteDependency(ComponentGroup<CameraComponent>());
	AddReadDependency(ComponentGroup<InputComponent>());
	AddWriteDependency(ComponentGroup<TransformComponent>());
}
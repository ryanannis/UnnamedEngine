#include "FreecamPlayerSystem.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Client/ClientInputManager.h"
#include "Engine/Base/Components/SingletonComponents/InputComponent.h"
#include "Engine/Base/Components/TransformComponent.h"
#include "Engine/Base/Client/GameFramework.h"
#include "Engine/Base/Client/Keycodes.h"

FreecamPlayerSystem::FreecamPlayerSystem(Ptr<Context> context) :
	mContext(context)
{}

void FreecamPlayerSystem::Update(float delta, Ptr<EntityAdmin> e)
{
	const float FREECAMSPEED = 10.0;  // 10m/s
	Ptr<const InputComponent> inputComponent = GetSingletonComponent<InputComponent>(e);
	const Entity& playerEntity = mContext->GetGameFramework()->GetGameClient()->GetLocalPlayerEntity();
	Ptr<TransformComponent> transformComponent = GetWriteComponent<TransformComponent>(e, playerEntity);

	for(auto input : inputComponent->inputEvents)
	{
		if(input.GetKeycode() == KEY_W)
		{
			transformComponent->pEntityWorldTranslation += Vector3f(0, 0, FREECAMSPEED) * delta;
		}
		else if(input.GetKeycode() == KEY_S)
		{
			transformComponent->pEntityWorldTranslation += Vector3f(0, 0, -FREECAMSPEED) * delta;
		}
		else if(input.GetKeycode() == KEY_A)
		{
			transformComponent->pEntityWorldTranslation += Vector3f(-FREECAMSPEED, 0, 0) * delta;
		}
		else if(input.GetKeycode() == KEY_D)
		{
			transformComponent->pEntityWorldTranslation += Vector3f(FREECAMSPEED, 0, 0) * delta;
		}
	}
}

void FreecamPlayerSystem::StaticInitDependencies()
{
	AddReadDependency(ComponentGroup<InputComponent>());
	AddWriteDependency(ComponentGroup<TransformComponent>());
}
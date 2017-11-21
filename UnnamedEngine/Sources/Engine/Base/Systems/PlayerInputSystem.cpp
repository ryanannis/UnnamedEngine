#include "PlayerInputSystem.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Client/ClientInputManager.h"
#include "Engine/Base/Components/InputComponent.h"

PlayerInputSystem::PlayerInputSystem(Ptr<Context> context) :
	mContext(context)
{}

void PlayerInputSystem::Update(float, Ptr<EntityAdmin>)
{
	Ptr<ClientInputManager> manager = mContext->GetInputManager();
	while(manager->HasInput())
	{
		manager->GetInputEvent();
	}
}

void PlayerInputSystem::StaticInitDependencies()
{

}
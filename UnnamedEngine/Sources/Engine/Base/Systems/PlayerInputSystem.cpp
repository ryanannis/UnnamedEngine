#include "PlayerInputSystem.h"

#include "Engine/Base/Managers/EntityAdmin.h"
#include "Engine/Base/Client/Context.h"

PlayerInputSystem::PlayerInputSystem(Ptr<Context> context) :
	mContext(context)
{}


void PlayerInputSystem::Update(float, Ptr<EntityAdmin>)
{
}

void PlayerInputSystem::StaticInitDependencies()
{

}
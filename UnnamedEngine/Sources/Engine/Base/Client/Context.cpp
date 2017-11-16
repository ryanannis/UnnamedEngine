#include "Context.h"

#include "Engine/Base/Client/Client.h"
#include "Engine/Base/Client/GameFramework.h"

Context::Context()
{

}

Context::~Context()
{

}

Ptr<Client> Context::GetClient()
{
	return(mClient);
}

Ptr<ClientInputManager> Context::GetInputManager()
{
	return(mClientInputManager);
}

Ptr<ResourceManager> Context::GetResourceManager()
{
	return(GetClient()->GetTarget()->GetResourceManager());
}

Ptr<GameFramework> Context::GetGameFramework()
{
	return(GetClient()->GetTarget());
}

Ptr<Renderer> Context::GetRenderer()
{
	return(GetGameFramework()->GetRenderer());
}
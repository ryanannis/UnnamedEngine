#include "Context.h"

Context::Context()
{

}

Context::~Context()
{

}

void Context::SetClient(Ptr<Client> client)
{
	mClient = client;
}

Ptr<Client> Context::GetClient()
{
	return(mClient);
}

void Context::SetInputManager(Ptr<ClientInputManager> inputManager)
{
	mClientInputManager = inputManager;
}

Ptr<ClientInputManager> Context::GetInputManager()
{
	return(mClientInputManager);
}


#include "Context.h"

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


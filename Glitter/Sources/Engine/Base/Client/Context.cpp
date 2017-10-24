#include "Context.h"

Context::Context()
{

}

Context::~Context()
{

}

void Context::SetClient(Client* client)
{
	mClient = client;
}


Client* Context::GetClient()
{
	return(mClient);
}
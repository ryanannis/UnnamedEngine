#pragma once
#include "Engine/Base/Common/Common.h"

class Client;
class RegionAdmin;
class ClientInputManager;

class Context : public NonCopyable
{
public:
	Context();
	~Context();

	void SetClient(Ptr<Client> client);
	Ptr<Client> GetClient();

	void SetInputManager(Ptr<ClientInputManager> inputManager);
	Ptr<ClientInputManager> GetInputManager();

private:
	Ptr<Client> mClient;
	Ptr<ClientInputManager> mClientInputManager;
	Ptr<RegionAdmin> mRegionAdmin;

};
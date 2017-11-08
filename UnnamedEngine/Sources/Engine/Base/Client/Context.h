#pragma once
#include "Engine/Base/Common/Common.h"

class Client;
class RegionAdmin;
class GameFramework;
class ClientInputManager;

class Context : public NonCopyable
{
public:
	Context();
	~Context();

	Ptr<Client> GetClient();
	Ptr<ClientInputManager> GetInputManager();

private:
	Ptr<Client> mClient;
	Ptr<ClientInputManager> mClientInputManager;
	Ptr<RegionAdmin> mRegionAdmin;

	// An unfortunate friendship, but better than having setters
	// all over the goddamn place or reconstructing every time we
	// want to change.
	friend Client;
	friend GameFramework;
};
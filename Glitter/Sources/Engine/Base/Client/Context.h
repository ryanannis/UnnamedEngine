#pragma once
#include "Engine/Base/Common/Common.h"

class Client;
class RegionAdmin;

class Context
{
public:
	Context();
	~Context();

	void SetClient(Client* client);
	Client* GetClient();

	void SetRegionAdmin(Ptr<RegionAdmin> regionAdmin) { mRegionAdmin = regionAdmin };

private:
	Client* mClient;
	Ptr<RegionAdmin> mRegionAdmin;

};
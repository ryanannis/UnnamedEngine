#pragma once

class Client;

class Context
{
public:
	Context();
	~Context();

	void SetClient(Client* client);
	Client* GetClient();

private:
	Client* mClient;


};
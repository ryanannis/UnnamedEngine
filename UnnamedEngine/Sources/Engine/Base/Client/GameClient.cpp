#include "GameClient.h"

GameClient::GameClient(Ptr<Context> context)
{
	mContext = context;
}

void GameClient::SetLocalPlayerEntity(const Entity& e)
{
	mLocalPlayerEntity = e;
}

const Entity& GameClient::GetLocalPlayerEntity()
{
	return(mLocalPlayerEntity);
}
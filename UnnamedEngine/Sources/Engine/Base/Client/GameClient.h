#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Entity.h"

class Context;

class GameClient
{
public:
	GameClient(Ptr<Context> context);

	void SetLocalPlayerEntity(const Entity& e);
	const Entity& GetLocalPlayerEntity();

private:
	Ptr<Context> mContext;

	Entity mLocalPlayerEntity;
};
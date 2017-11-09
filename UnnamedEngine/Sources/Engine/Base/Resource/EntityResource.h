#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/PropTree.h"

class EntityResource : public Resource
{
public:
	EntityResource(std::string uri);
	virtual bool IsReady() const override;
	virtual void Load() override;

private:
	bool mReady;
};
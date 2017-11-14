#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/PropTree.h"
#include "Engine/Base/Types/StaticComponent.h"

class EntityAdmin;
class LevelObject;

class LevelResource : public Resource
{
public:
	LevelResource(std::string uri);
	virtual bool IsReady() const override;
	virtual void Load(Ptr<ResourceManager> manager) override;

private:
	std::vector<LevelObject> mLevelObjects;
	bool mReady;
};

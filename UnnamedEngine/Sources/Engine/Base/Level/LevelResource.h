#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/PropTree.h"
#include "Engine/Base/Types/StaticComponent.h"
#include "Engine/Base/Level/LevelObject.h"

class EntityAdmin;

class LevelResource : public Resource
{
public:
	LevelResource(std::string uri);
	virtual bool IsReady() const override;
	virtual void Load(Ptr<ResourceManager> manager) override;
	const std::vector<LevelObject>& GetLevelObjects() const;
	const std::vector<std::weak_ptr<EntityResource>>& GetLevelObjectResources() const;

private:
	std::vector<std::weak_ptr<EntityResource>> mLevelObjectResources;
	std::vector<LevelObject> mLevelObjects;
	bool mReady;
};

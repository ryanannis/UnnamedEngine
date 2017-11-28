#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/PropTree.h"
#include "Engine/Base/Types/StaticComponent.h"

class EntityAdmin;

class EntityResource : public Resource
{
public:
	EntityResource(URI uri);
	virtual bool IsReady() const override;
	virtual void Load(Ptr<ResourceManager> manager) override;
	Entity ConstructEntity(EntityAdmin& admin);

private:
	bool mReady;
	std::vector<
		std::pair<
			Ptr<const StaticReg::StaticRegistryEntry>,
			std::unique_ptr<ComponentBase>
		>
	> mConstructionInfo;
};

#include "Engine/Base/Resource/EntitySchema.h"
#include "Engine/Base/Managers/EntityAdmin.h"

class AvatarSchema : public EntitySchema
{
	virtual std::string GetSchemaName() const override;
	virtual void ProcessEntity(const PropTree& propTree, Ptr<EntityAdmin> entityAdmin, Entity entity) override;
	
};
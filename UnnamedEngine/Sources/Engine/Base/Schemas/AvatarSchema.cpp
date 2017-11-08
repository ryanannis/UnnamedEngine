#include "AvatarSchema.h"
#include "Engine/Base/Components/IdentityComponent.h"
#include "Engine/Base/Components/HealthComponent.h"
#include <string>

struct AvatarSchemaMap
{
	
};

std::string AvatarSchema::GetSchemaName() const
{

}

void AvatarSchema::ProcessEntity(const PropTree& propTree, Ptr<EntityAdmin> entityAdmin, Entity entity)
{
	// todo:  automate this awful stuff -- also we should also only need to 
	// do the unordered_map lookups once when loading in the type
	auto name = propTree.leaves.find("Name");
	auto maxHealth = propTree.leaves.find("Max Health");
	auto mesh = propTree.leaves.find("Mesh Path:");
	assert(name != propTree.leaves.end());
	assert(maxHealth != propTree.leaves.end());
	assert(mesh != propTree.leaves.end());

	entityAdmin->AddComponent<IdentityComponent>(entity, name->second);
	entityAdmin->AddComponent<HealthComponent>(entity, std::stoi(maxHealth->second));

};
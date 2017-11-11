#include "IdentityComponent.h"
#include "Engine/Base/Resource/Serializer.h"
#include "Engine/Base/Resource/PropTree.h"

Serializer& IdentityComponent::Serialize(Serializer& s)
{
	s.Serialize("Name", entityName);
	return(s);
}

void IdentityComponent::Deserialize(PropTree& t)
{
	auto nameIt = t.leaves.find("Name");

	if(nameIt != t.leaves.end())
	{
		entityName = nameIt->second.GetAsString();
	}
}

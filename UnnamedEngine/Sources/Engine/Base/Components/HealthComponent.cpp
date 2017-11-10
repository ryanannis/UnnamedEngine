#include "HealthComponent.h"
#include "Engine/Base/Resource/Serializer.h"
#include "Engine/Base/Resource/PropTree.h"

Serializer& HealthComponent::Serialize(Serializer& s)
{
	s.Serialize("MaxHealth", maxHealth);
	s.Serialize("StartingHealth", currentHealth);
	return(s);
}

void HealthComponent::Deserialize(PropTree& t)
{
	auto maxHealth = t.leaves.find("MaxHealth");
	auto currentHealth = t.leaves.find("MaxHealth");
	
	if(maxHealth != t.leaves.end())
	{
		if(auto maxHP = maxHealth->second.GetAsInt()) {
			maxHealth = *maxHP;
		}
	}
	
	if(currentHealth != t.leaves.end())
	{
		if(auto curHP = maxHealth->second.GetAsInt())
		{
			maxHealth = *curHP;
		}
	}
}
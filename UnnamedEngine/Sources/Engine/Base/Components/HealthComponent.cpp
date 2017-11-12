#include "HealthComponent.h"
#include "Engine/Base/Resource/Serializer.h"
#include "Engine/Base/Resource/PropTree.h"

STATICDEF(HealthComponent, "HealthComponent");

HealthComponent::HealthComponent() :
	maxHealth{100},
	currentHealth{100}
{}

Serializer& HealthComponent::Serialize(Serializer& s)
{
	s.Serialize("MaxHealth", maxHealth);
	s.Serialize("StartingHealth", currentHealth);
	return(s);
}

void HealthComponent::Deserialize(const DeserializationData& s)
{
	PropTree t = s.GetProps();
	auto maxHealthIt = t.leaves.find("MaxHealth");
	auto currentHealthIt = t.leaves.find("MaxHealth");
	
	if(maxHealthIt != t.leaves.end())
	{
		if(auto maxHP = maxHealthIt->second.GetAsInt()) {
			maxHealth = *maxHP;
		}
	}
	
	if(currentHealthIt != t.leaves.end())
	{
		if(auto curHP = currentHealthIt->second.GetAsInt())
		{
			currentHealth = *curHP;
		}
	}
}
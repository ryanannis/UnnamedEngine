#include "HealthComponent.h"
#include "Engine/Base/Resource/Serializer.h"
#include "Engine/Base/Resource/PropTree.h"

STATICREGISTER(HealthComponent, "HealthComponent");

HealthComponent::HealthComponent() :
	maxHealth{100},
	currentHealth{100}
{}

Serializer& HealthComponent::Serialize(Serializer& s)
{
	SERIALIZE(s, "MaxHealth", maxHealth);
	SERIALIZE(s, "StartingHealth", currentHealth);
	return(s);
}

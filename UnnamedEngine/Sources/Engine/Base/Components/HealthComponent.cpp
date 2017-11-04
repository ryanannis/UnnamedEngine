#include "HealthComponent.h"
#include "Engine/Base/Resource/Serializer.h"

Serializer& HealthComponent::Serialize(Serializer& s)
{
	s.Serialize("Max Health", mMaxHealth);
	s.Serialize("Current Health", mCurrentHealth);
	return(s);
}
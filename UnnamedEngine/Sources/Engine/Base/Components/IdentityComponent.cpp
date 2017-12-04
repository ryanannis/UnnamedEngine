#include "IdentityComponent.h"
#include "Engine/Base/Resource/Serializer.h"
#include "Engine/Base/Resource/PropTree.h"

STATICREGISTER(IdentityComponent, "IdentityComponent");

IdentityComponent::IdentityComponent() :
	entityName{"NoName"}
{}

Serializer& IdentityComponent::Serialize(Serializer& s)
{
	SERIALIZE(s, "Name", entityName);
	return(s);
}

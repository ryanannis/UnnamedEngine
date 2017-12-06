#include "TransformComponent.h"

STATICREGISTER(TransformComponent, "TransformComponent");

TransformComponent::TransformComponent() :
	pEntityWorldTranslation{},
	pEntityWorldRotation{}
{
}

Serializer& TransformComponent::Serialize(Serializer& s)
{
	SERIALIZE(s, "", pEntityWorldTranslation);
	//SERIALIZE(s, "", pEntityWorldRotation);
	return(s);
}

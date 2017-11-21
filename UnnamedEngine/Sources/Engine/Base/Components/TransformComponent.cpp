#include "TransformComponent.h"

STATICREGISTER(TransformComponent, "TransformComponent");

TransformComponent::TransformComponent() :
	pEntityWorldTranslation{},
	pEntityWorldRotation{}
{
}


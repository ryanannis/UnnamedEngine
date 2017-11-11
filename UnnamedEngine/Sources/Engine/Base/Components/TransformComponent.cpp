#include "TransformComponent.h"

TransformComponent::TransformComponent() :
	pEntityWorldTranslation{},
	pEntityWorldRotation{}
{
}

STATICREGISTER(TransformComponent, "TransformComponent");
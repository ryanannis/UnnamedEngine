#include "CameraComponent.h"
#include "Engine/Base/Resource/Serializer.h"

STATICREGISTER(CameraComponent, "CameraComponent")

CameraComponent::CameraComponent() :
	pEntityCameraTranslation{},
	pCameraRotation{}
{}

Serializer& CameraComponent::Serialize(Serializer& s)
{
	SERIALIZE(s, "", pEntityCameraTranslation);
	SERIALIZE(s, "", pCameraRotation);
	return(s);
}
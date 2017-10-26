#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"

class CameraComponent : public Component
{
public:
	glm::fvec4 pEntityCameraTranslation;
	glm::fquat pEntityCameraRotation;
};
#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"

class TransformComponent : public Component<TransformComponent>
{
public:
	glm::fvec4 pEntityCameraTranslation;
	glm::fquat pEntityCameraRotation;
};
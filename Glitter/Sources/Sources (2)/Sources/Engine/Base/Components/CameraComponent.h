#pragma once

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Engine/Base/Types/Component.h"

class TransformComponent : public Component
{
public:
	glm::fvec4 pEntityCameraTranslation;
	glm::fquat pEntityCameraRotation;
};
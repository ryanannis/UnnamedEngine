#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Base/Types/Component.h"

class TransformComponent : public Component
{
public:
	glm::fvec4 pEntityWorldTranslation;
	glm::fquat pEntityWorldRotation;
};
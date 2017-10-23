#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"

class TransformComponent : public Component
{
public:
	glm::fvec4 pEntityWorldTranslation;
	glm::fquat pEntityWorldRotation;
};
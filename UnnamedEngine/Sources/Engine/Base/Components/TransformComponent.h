#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/StaticComponent.h"

class TransformComponent : public Component<TransformComponent>
{
public:
	TransformComponent();

	glm::fvec4 pEntityWorldTranslation;
	glm::fquat pEntityWorldRotation;
};

template<> std::string Component<TransformComponent>::sName = "TransformComponent";
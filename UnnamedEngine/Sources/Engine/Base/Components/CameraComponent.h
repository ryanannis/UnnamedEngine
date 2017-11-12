#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/StaticComponent.h"

class CameraComponent : public Component<CameraComponent>
{
public:
	CameraComponent();

	glm::fvec3 pEntityCameraTranslation;
	glm::fquat pEntityCameraRotation;
};

template<> std::string Component<CameraComponent>::sName = "HealthComponent";

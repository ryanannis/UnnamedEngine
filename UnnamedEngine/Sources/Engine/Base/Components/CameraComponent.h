#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"

class CameraComponent : public Component<CameraComponent>
{
public:
	glm::fvec4 pEntityCameraTranslation;
	glm::fquat pEntityCameraRotation;
};

template<> std::string Component<CameraComponent>::sName = "CameraComponent";

Serializer Serialize(CameraComponent& c);
#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/StaticComponent.h"

class CameraComponent : public Component<CameraComponent>
{
public:
	CameraComponent();

	Vector3f pEntityCameraTranslation;
	Vector3f pCameraRotation;

	virtual Serializer& Serialize(Serializer& s) override;
};

template<> std::string Component<CameraComponent>::sName = "HealthComponent";
template<> StorageStrategy Component<CameraComponent>::sStorageStrategy = StorageStrategy::HashMap;

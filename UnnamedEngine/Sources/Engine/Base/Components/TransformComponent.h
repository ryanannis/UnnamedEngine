#pragma once

#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Resource/PropTree.h"
#include "Engine/Base/Types/StaticComponent.h"
#include "Engine/Base/Resource/Serializer.h"


class TransformComponent : public Component<TransformComponent>
{
public:
	TransformComponent();

	glm::fvec3 pEntityWorldTranslation;
	glm::fquat pEntityWorldRotation;

	virtual Serializer& Serialize(Serializer& s) override;
};

template<> std::string Component<TransformComponent>::sName = "TransformComponent";
template<> StorageStrategy Component<TransformComponent>::sStorageStrategy = StorageStrategy::Consecutive;

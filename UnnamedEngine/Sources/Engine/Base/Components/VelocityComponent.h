#pragma once

#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Resource/PropTree.h"
#include "Engine/Base/Types/StaticComponent.h"

class VelocityComponent : public Component<VelocityComponent>
{
public:
	VelocityComponent();

	Vector3f pVelocity;
	Vector3f pAngularVelocity;
};

template<> std::string Component<VelocityComponent>::sName = "VelocityComponent";
template<> StorageStrategy Component<VelocityComponent>::sStorageStrategy = StorageStrategy::Consecutive;

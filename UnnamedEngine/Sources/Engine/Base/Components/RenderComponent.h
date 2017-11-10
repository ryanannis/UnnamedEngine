#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Resource/MeshResource.h"

class RenderComponent : Component<RenderComponent> {

	std::weak_ptr<MeshResource> mesh;
};

template<> std::string Component<RenderComponent>::sName = "RenderComponent";

Serializer Serialize(RenderComponent& c);
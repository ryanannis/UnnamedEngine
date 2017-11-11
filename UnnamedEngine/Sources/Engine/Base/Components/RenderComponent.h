#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Types/StaticComponent.h"
#include "Engine/Base/Resource/MeshResource.h"

struct PropTree;

class RenderComponent : public Component<RenderComponent>
{
public:
	RenderComponent();
	ResourceType<MeshResource> mesh;

	Serializer& Serialize(Serializer& s);
	void Deserialize(PropTree& t);
};

template<> std::string Component<RenderComponent>::sName = "RenderComponent";

#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/MeshResource.h"
#include "Engine/Base/Types/StaticComponent.h"

struct PropTree;

class RenderComponent : public Component<RenderComponent>
{
public:
	RenderComponent();
	ResourceType<MeshResource> mesh;

	Serializer& Serialize(Serializer& s);
	void Deserialize(const PropTree& t);
};

STATICDECL(RenderComponent, "RenderComponent");

template<> std::string Component<RenderComponent>::sName = "RenderComponent";

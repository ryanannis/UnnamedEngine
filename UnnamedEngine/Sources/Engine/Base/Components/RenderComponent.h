#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Resource/MeshResource.h"

struct PropTree;

class RenderComponent : Component<RenderComponent>
{
public:
	MeshResource mesh;

	Serializer& Serialize(Serializer& s);
	void Deserialize(PropTree& t);
};

template<> std::string Component<RenderComponent>::sName = "RenderComponent";

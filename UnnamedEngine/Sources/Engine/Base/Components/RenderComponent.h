#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/ModelResource.h"
#include "Engine/Base/Types/StaticComponent.h"

struct PropTree;

class RenderComponent : public Component<RenderComponent>
{
public:
	RenderComponent();
	ResourceType<ModelResource> mesh;
	GraphicsHandle handle;


	Serializer& Serialize(Serializer& s);
};

template<> std::string Component<RenderComponent>::sName = "RenderComponent";
template<> StorageStrategy Component<RenderComponent>::sStorageStrategy = StorageStrategy::HashMap;


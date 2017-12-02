#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/ModelResource.h"

struct GraphicsPacket
{
public:
	ResourceType<ModelResource> mesh;
	Vector3f translation;
	Vector3f rotation;
};

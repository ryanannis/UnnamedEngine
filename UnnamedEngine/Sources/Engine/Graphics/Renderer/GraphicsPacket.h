#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/MeshResource.h"

struct GraphicsPacket
{
public:
	ResourceType<MeshResource> mesh;
	Vector3f translation;
	Vector3f rotation;
};

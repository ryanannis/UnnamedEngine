#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Resource/EntityResource.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/PropTree.h"

// An object that is statically placed in the level with a map editor.
class LevelObject
{
public:
	LevelObject(const PropTree& tree);
	ResourceType<EntityResource> GetResource();
	Vector3f GetPosition();
	Vector3f GetEulerRotation();

private:
	ResourceType<EntityResource> mResource;
	Vector3f mPosition;
	Vector3f mRotation;
};
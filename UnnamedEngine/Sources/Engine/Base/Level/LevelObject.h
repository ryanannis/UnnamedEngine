#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Base/Resource/EntityResource.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/PropTree.h"

// An object that is statically placed in the level with a map editor.
class LevelObject
{
public:
	LevelObject(PropTree& tree);
	ResourceType<EntityResource> GetResource();
	Vector3f GetPosition();
	Quat GetRotation();

private:
	ResourceType<EntityResource> mRes;
	Vector3f mPosition;
	Quat mRotiation;

};
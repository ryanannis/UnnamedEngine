#pragma once

#include "Engine/Base/Resource/EntityResource.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/PropTree.h"

// An object that is statically placed in the level with a map editor.
class LevelObject
{
	LevelObject(PropTree& tree);
	ResourceType<EntityResource> GetResource
};
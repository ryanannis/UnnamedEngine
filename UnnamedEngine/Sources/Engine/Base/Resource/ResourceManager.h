#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/MeshResource.h"


#include <string>
#include <unordered_map>
#include <type_traits>

class ResourceManager
{
public:
	std::weak_ptr<MeshResource> LoadMeshResource(std::string& URI);
	

private:
	std::unordered_map<std::string, std::shared_ptr<MeshResource>> mMeshResources;
};
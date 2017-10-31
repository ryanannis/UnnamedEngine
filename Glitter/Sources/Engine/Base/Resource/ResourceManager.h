#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/Resource.h"

#include <string>
#include <unordered_map>

class ResourceManager
{
public:
	Resource* SpotLoadResource(const std::string dataLocation);
	Resource* LoadResourceAsync(const std::string dataLocation);

private:
	//std::unordered_map<std::string, Resource> mLoadedResources;
};
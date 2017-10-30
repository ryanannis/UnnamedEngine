#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/Resource.h"

#include <string>
#include <unordered_map>


enum class LoadStyle
{
	SPOT_LOAD,
	DEFERRED
};

class ResourceLoader
{
public:
	Resource* LoadResource(const std::string dataLocation, LoadStyle style);

private:
	std::unordered_map<std::string, Resource> mLoadedResources;
};
#include "ResourceManager.h"

Resource* ResourceManager::LoadResource(const std::string dataLocation, LoadStyle style)
{
	assert(style == LoadStyle::DEFERRED); // not implemented yet
	return(nullptr);
}

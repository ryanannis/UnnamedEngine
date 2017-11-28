#pragma once

#include <string>
#include "Engine/Base/Resource/URI.h"

class Serializer;
class ResourceManager;

// You can feed this to the ResourceManager and get a real resource.
// The important thing is the type since you can get back a Resource
// of type T.
template <typename T>
class ResourceType
{
public:
	ResourceType() :
		mURI{}
	{}

	explicit ResourceType(URI uri) : 
		mURI{uri}
	{}

	explicit ResourceType(std::string uri) :
		mURI{URI(uri)}
	{}


private:
	URI mURI;

	friend Serializer;
	friend ResourceManager;
};
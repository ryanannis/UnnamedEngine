#pragma once

#include <string>

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

	ResourceType(std::string uri) : 
		mURI{uri}
	{}


private:
	std::string mURI;

	friend Serializer;
	friend ResourceManager;
};
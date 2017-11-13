#pragma once

#include <string>

class ResourceManager;

/*
 * A resource is something that can be used as a factory to construct a runtime type
 * or contains information used by a system (eg. a MeshResource)
 */
class Resource
{
public:
	Resource(std::string uri) : mUri(uri) {};
	virtual bool IsReady() const = 0;
	virtual void Load(Ptr<ResourceManager> manager )= 0;
	std::string GetURI() const { return(mUri); }

private:
	std::string mUri;

	friend ResourceManager;
};
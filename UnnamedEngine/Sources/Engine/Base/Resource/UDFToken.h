#pragma once

#include <string>

class ResourceManager;

/*
 * A resource is something that can be used as a factory to construct a runtime type
 * or contains information used by a system (eg. a ModelResource)
 */
class Resource
{
public:
	Resource(std::string uri) : mUri(uri) {};
	virtual bool IsReady() const = 0;
	virtual void Load() = 0;
	std::string GetURI() const { return(mUri); }

private:
	// This is set immediately upon entering the resource manager
	// as Resource file also act as loaders and we of course want
	// to abstract factrthis away from the user.
	Ptr<ResourceManager> mResourceManager;
	std::string mUri;

	friend ResourceManager;
};
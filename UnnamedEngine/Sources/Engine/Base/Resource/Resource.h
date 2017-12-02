#pragma once

#include <string>
#include "Engine/Base/Resource/URI.h"

class ResourceManager;

/*
 * A resource is something that can be used as a factory to construct a runtime type
 * or contains information used by a system (eg. a ModelResource)
 */
class Resource
{
public:
	Resource(URI uri) : mUri(uri) {};
	virtual bool IsReady() const = 0;
	virtual void Load(Ptr<ResourceManager> manager )= 0;
	const URI& GetURI() const { return(mUri); }

private:
	URI mUri;

	friend ResourceManager;
};
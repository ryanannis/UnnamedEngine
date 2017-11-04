#pragma once

#include <string>

/*
 * A resource is something that can be used as a factory to construct a runtime type
 * or contains information used by a system (eg. a MeshResource)
 */
class Resource
{
public:
	Resource(std::string URI) : URI(URI) {};
	virtual bool IsReady() const = 0;
	virtual void Load() = 0;
	std::string GetURI() const { return(URI); };

private:
	std::string URI;
};
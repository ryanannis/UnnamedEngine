#pragma once

class ResourcePtr
{
public:
	Resource(std::string uri) : mUri(uri) {};
	virtual bool IsReady() const = 0;
	virtual void Load() = 0;
	std::string GetURI() const { return(mUri); }
}
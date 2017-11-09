#include "ResourceManager.h"

#include <fstream>
#include <sstream>

static const std::string DATA_FILE_EXTENSION = "udf";

ResourceManager::ResourceManager(Ptr<Context> context) :
	mContext(context)
{}

std::weak_ptr<MeshResource> ResourceManager::LoadMeshResource(std::string& URI)
{
	auto val = mMeshResources.find(URI);
	if(val != mMeshResources.end())
	{
		std::weak_ptr<MeshResource> wkResouce = val->second;
		return(wkResouce);
	}

	std::shared_ptr<MeshResource> res = std::make_shared<MeshResource>(URI);
	std::weak_ptr<MeshResource> wkResouce = res;
	res->Load();
	mMeshResources.emplace(URI, std::move(res));

	return(res);
}

URI ResourceManager::ParseStringToURI(const std::string strURI)
{
	// This is remarkably inefficient but it shouldn't matter
	URI uri;

	// Split string
	std::vector<std::string> split;
	std::stringstream ss(strURI);
	std::string item;
	while(getline(ss, item, '/')){
		split.push_back(item);
	}

	// We consider the path, [split] from 0 to pathIndex
	size_t pathIndex = 0;

	if(split.size() > 1)
	{
		std::string lastToken = split[split.size() - 1];
		std::string secondLastToken = split[split.size() - 2];
		size_t dotLocation = lastToken.find('.');
		
		// No dot - must be a serialized resource
		if(dotLocation == std::string::npos)
		{
			uri.component = lastToken;
			uri.ext = DATA_FILE_EXTENSION;
			uri.file = secondLastToken;
			pathIndex = split.size() - 2;
		}
		else
		{
			uri.ext = lastToken.substr(dotLocation + 1);
			uri.file = lastToken.substr(0, dotLocation);
			pathIndex = split.size() - 1;
		}

		std::stringstream restOfPath;
		for(size_t i = 0; i < pathIndex; i++)
		{
			restOfPath << split[i] << '/';
		}
		uri.path = restOfPath.str();
	}

	return(uri);
}
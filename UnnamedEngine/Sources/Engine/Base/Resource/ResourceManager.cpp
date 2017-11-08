#include "ResourceManager.h"

#include <fstream>
#include <sstream>

static const std::string DATA_FILE_EXTENSION = ".udf";

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

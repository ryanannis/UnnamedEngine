#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/MeshResource.h"

#include <string>
#include <unordered_map>
#include <type_traits>

class Context;

struct URI
{
	std::string path;
	std::string file;
	std::string ext;
	std::string component;
};

class ResourceManager
{
public:
	ResourceManager(Ptr<Context> context);
	std::weak_ptr<MeshResource> LoadMeshResource(std::string& URI);
	
	static URI ParseStringToURI(const std::string strURI);
	
	
private:
	std::unordered_map<std::string, std::shared_ptr<MeshResource>> mMeshResources;
	Ptr<Context> mContext;
};
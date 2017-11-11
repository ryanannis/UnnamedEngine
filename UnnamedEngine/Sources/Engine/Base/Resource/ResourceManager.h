#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/MeshResource.h"
#include "Engine/Base/Resource/ResourceType.h"

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
	static URI ParseStringToURI(const std::string strURI);

	template <typename T>
	std::weak_ptr<T> LoadResource(ResourceType<T> resourceType)
	{
		// Check for existence of resource on the flywheel
		auto resIt = mResources.find(resourceType.mURI);
		if(resIt != mResources.end())
		{
			return(resIt->second);
		}

		// The emplace creates real knarly compiler errors otherwise
		static_assert
		(
			std::is_base_of<Resource, T>::value,
			"Attempted to load non-resource as resource!"
		);
		auto res = make_shared<T>{resourceType.mURI};
		res.Load();
		mResources.emplace{mURI, res};
		return(res);
	}
	
private:
	std::unordered_map<std::string, std::shared_ptr<Resource>> mResources;
	Ptr<Context> mContext;
};
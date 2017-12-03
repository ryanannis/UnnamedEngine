#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/ModelResource.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/URI.h"

#include <string>
#include <unordered_map>
#include <type_traits>

class Context;

class ResourceManager
{
public:
	ResourceManager(Ptr<Context> context);

	//todo: static tables for each resource type - don't like the dynamic_cast
	template <typename T>
	std::shared_ptr<T> LoadResource(ResourceType<T> resourceType)
	{
		// The emplace creates real knarly compiler errors otherwise
		static_assert
		(
			std::is_base_of<Resource, T>::value,
			"Attempted to load non-resource as resource!"
		);

		// Check for existence of resource on the flywheel
		auto resIt = mResources.find(resourceType.mURI.GetHash());
		if(resIt != mResources.end())
		{	
			if(!resIt->second.expired())
			{
				std::shared_ptr<Resource> wkres = resIt->second.lock();
				return(std::dynamic_pointer_cast<T>(wkres));
			}
			else
			{
				std::shared_ptr<T> res = std::make_shared<T>(resourceType.mURI);
				res->Load(this);

				std::weak_ptr<T> wkres;
				wkres = res;
				mResources.emplace(resourceType.mURI.GetHash(), wkres);

				return(res);
			}
		}

		std::shared_ptr<T> res = std::make_shared<T>(resourceType.mURI);
		res->Load(this);

		std::weak_ptr<T> wkres;
		wkres = res;
		mResources.emplace(resourceType.mURI.GetHash(), wkres);

		return(res);
	}
	
private:
	std::unordered_map<std::string, std::weak_ptr<Resource>> mResources;
	Ptr<Context> mContext;
};
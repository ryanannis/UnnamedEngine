#include "LevelResource.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Engine/Base/Resource/Serializer.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/PropParser.h"
#include "Engine/Base/Managers/EntityAdmin.h"

LevelResource::LevelResource(URI uri) :
	Resource(uri),
	mReady(false)
{};

bool LevelResource::IsReady() const
{
	return(mReady);
}

void LevelResource::Load(Ptr<ResourceManager> resourceManager)
{
	const URI uri = URI(GetURI());

	/* todo: stick this somewhere else (probably res manager)
	* as multiple types of resource are maybe dependent on a
	* single file in the future */
	std::string fullURL = uri.GetFilePath();
	std::ifstream f(fullURL);
	std::stringstream buffer;
	buffer << f.rdbuf();

	std::optional<PropTree> propTree = PropParser::Parse(buffer.str());
	
	// Load all of the objects in the level
	for(const auto& subTreePair: propTree->components)
	{
		const auto& subtree = subTreePair.second;
		mLevelObjects.push_back(LevelObject(subtree));
	}

	// Load the resources associated with objects
	for(auto& levelObject : mLevelObjects)
	{
		auto loadedResource = resourceManager->LoadResource(levelObject.GetResource());
		mLevelObjectResources.push_back(loadedResource);
	}

	mReady = true;
}

const std::vector<LevelObject>& LevelResource::GetLevelObjects() const
{
	assert(mReady);
	return(mLevelObjects);
}

const std::vector<std::weak_ptr<EntityResource>>& LevelResource::GetLevelObjectResources() const
{
	assert(mReady);
	return(mLevelObjectResources);
}
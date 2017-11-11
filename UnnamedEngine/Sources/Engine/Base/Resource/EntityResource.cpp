#include "EntityResource.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Engine/Base/Resource/Serializer.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/PropParser.h"

EntityResource::EntityResource(std::string uri) :
	Resource(uri),
	mReady(false)
{};

bool EntityResource::IsReady() const
{
	return(mReady);
}

void EntityResource::Load()
{
	const URI uri = ResourceManager::ParseStringToURI(GetURI());

	/* todo: stick this somewhere else (probably res manager)
	 * as multiple types of resource can be dependent on a single file*/
	std::ifstream f(uri.file + uri.file + uri.ext);
	std::stringstream buffer;
	buffer << f.rdbuf();
	
	std::optional<PropTree> propTree = PropParser::Parse(buffer.str());

	if(propTree.has_value())
	{
		const auto& componentList = propTree->components;
		auto component = componentList.find(uri.component);
		if(component != componentList.end())
		{
			Serializer(component->second);
			
			mReady = true;
		}
		else
		{
			assert(false); // failed to load
		}
	}
	else
	{
		assert(false);
	}
}
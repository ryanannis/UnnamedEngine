#include "EntityResource.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Engine/Base/Resource/Serializer.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/PropParser.h"
#include "Engine/Base/Managers/EntityAdmin.h"

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
	const URI uri = URI(GetURI());

	/* todo: stick this somewhere else (probably res manager)
	 * as multiple types of resource are maybe dependent on a 
	  * single file in the future */
	std::string fullURL = uri.GetFilePath();
	std::ifstream f(fullURL);
	std::stringstream buffer;
	buffer << f.rdbuf();
	
	std::optional<PropTree> propTree = PropParser::Parse(buffer.str());

	if(!propTree.has_value())
	{
		assert(false);
	}

	const auto& schemaList = propTree->components;
	const auto& resSchema = schemaList.find(uri.GetComponent());
	
	if(resSchema == schemaList.end())
	{
		assert(false);
	}

	// List of components beloning to the entity we are trying to intializ
	const auto& componentList = resSchema->second.components;
	for(const auto& component : componentList)
	{
		const auto& componentName = component.first;
		const auto& componentTree = component.second;
		const auto registryData = StaticReg::GetComponentRegistryInformation(componentName);
		std::unique_ptr<ComponentBase> prototype = StaticReg::StaticCreateComponent(componentName);
		prototype->Deserialize(componentTree);
		mConstructionInfo.push_back(std::make_pair(registryData, std::move(prototype)));
	}
	mReady = true;
	
}

Entity EntityResource::ConstructEntity(EntityAdmin& admin)
{
	assert(mReady);

	Entity entity = admin.CreateEntity();
	
	for(const auto& registryInfo : mConstructionInfo)
	{
		const auto constructionInfo = registryInfo.first;
		const auto& prototype = registryInfo.second;
		Ptr<ComponentBase> component = admin.AddComponent(
			constructionInfo->flag, 
			constructionInfo->memSize,
			entity
		);
		std::memcpy(component.GetPtr(), prototype.get(), constructionInfo->memSize);
	}

	return(entity);
}
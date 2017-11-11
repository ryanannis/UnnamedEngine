#pragma once
#include "Engine/Base/Common/Common.h"

#include "PropTree.h"
#include "Engine/Base/Resource/ResourceType.h"

#include <string>
#include <vector>

class Resource;
class ComponentBase;
class EntityResource;

class Serializer
{
public:
	// Serializes with an empty proptree
	Serializer();

	Serializer& Serialize(ComponentBase& c);
	Serializer& Serialize(std::string tag, std::string s);
	Serializer& Serialize(std::string tag, int i);

	template <typename T>
	Serializer& Serialize(std::string tag, ResourceType<T>& res)
	{
		mSerializationTree.leaves.emplace(tag, CreateLeaf(Serialize(res)));
		return(*this);
	}

private:
	Serializer(PropTree t);

	std::string Serialize(std::string s) const;
	std::string Serialize(int i) const;
	
	template <typename T>
	std::string Serialize(ResourceType<T>& res) const
	{
		return(res.mURI);
	}

	PropTreeLeaf CreateLeaf(std::string s) const;
	
	PropTree mSerializationTree;

	friend EntityResource;
};

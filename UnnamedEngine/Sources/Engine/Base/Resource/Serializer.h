#pragma once
#include "Engine/Base/Common/Common.h"

#include "PropTree.h"

#include <string>
#include <vector>

class Resource;
class ComponentBase;

class Serializer
{
public:
	Serializer& Serialize(ComponentBase& c);
	Serializer& Serialize(std::string tag, std::string s);
	Serializer& Serialize(std::string tag, int i);
	Serializer& Serialize(std::string tag, Resource& res);

private:
	std::string Serialize(std::string s) const;
	std::string Serialize(int i) const;
	std::string Serialize(Resource& res) const;

	PropTreeLeaf CreateLeaf(std::string s) const;
	
	PropTree mSerializationTree;
};

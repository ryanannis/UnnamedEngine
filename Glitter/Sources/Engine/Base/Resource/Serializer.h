#pragma once
#include "Engine/Base/Common/Common.h"

#include<string>

class ComponentBase;

struct PropNode
{
	std::string key;
	std::string value;
	std::vector<PropNode> props;
};

class Serializer
{
public:
	Serializer& Serialize(ComponentBase& c);
	Serializer& Serialize(std::string tag, std::string s);
	Serializer& Serialize(std::string tag, int i);

private:
	std::string Serialize(std::string s) const;
	std::string Serialize(int i) const;

	PropNode GetNode() const;

	PropNode mSerializedNode;
};

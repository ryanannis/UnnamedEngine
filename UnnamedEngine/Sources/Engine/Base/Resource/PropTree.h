#pragma once
#include "Engine/Base/Common/Common.h"

struct PropTreeLeaf
{
public:
	std::string key;
	std::string value;
};

struct PropTree
{
public:
	PropTree(std::string name) : typeName{ name } {}
	std::string typeName;
	std::vector<PropTree> components;
	std::vector<PropTreeLeaf> leaves;
};
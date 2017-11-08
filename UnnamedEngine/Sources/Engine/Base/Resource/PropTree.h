#pragma once
#include "Engine/Base/Common/Common.h"
#include <unordered_map>

struct PropTree
{
public:
	PropTree() {}
	std::unordered_map<std::string, PropTree> components;
	std::unordered_map<std::string, std::string> leaves;
};
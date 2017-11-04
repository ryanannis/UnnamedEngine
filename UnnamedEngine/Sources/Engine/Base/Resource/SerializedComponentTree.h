#pragma once
#include "Engine/Base/Common/Common.h"

struct SerializedComponentProperty
{
	std::string componentFieldAlias;
	std::string value;
};

struct SerializedComponent
{
	std::string staticComponentName;
	std::vector<SerializedComponentProperty> componentProperties;
};

struct SerializedComponentTree
{
	std::string typeName;
	std::vector<SerializedComponent> components;
};
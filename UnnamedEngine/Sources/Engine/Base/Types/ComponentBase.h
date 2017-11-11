#pragma once
#include "Engine/Base/Common/Common.h"

class Serializer;
struct PropTree;

class ComponentBase {
public:
	virtual Serializer& Serialize(Serializer& s);
	virtual void Deserialize(PropTree& t);

	virtual std::string GetName() = 0;
};
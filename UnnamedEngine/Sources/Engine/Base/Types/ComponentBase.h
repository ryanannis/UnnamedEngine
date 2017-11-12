#pragma once
#include "Engine/Base/Common/Common.h"

class Serializer;
struct PropTree;

class ComponentBase {
public:
	virtual Serializer& Serialize(Serializer& s);

	//todo: replace PropTree with interface that has less stuff in it
	virtual void Deserialize(const PropTree& t);

	virtual std::string GetName() = 0;
};
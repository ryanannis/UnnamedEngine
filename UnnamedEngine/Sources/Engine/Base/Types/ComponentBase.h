#pragma once
#include "Engine/Base/Common/Common.h"

class Serializer;
class DeserializationData;

class ComponentBase {
public:
	virtual Serializer& Serialize(Serializer& s);

	//todo: replace PropTree with interface that has less stuff in it
	virtual void Deserialize(const DeserializationData& t);

	virtual std::string GetName() = 0;
};
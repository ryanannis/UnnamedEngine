#pragma once
#include "Engine/Base/Common/Common.h"

class Serializer;

enum class StorageStrategy
{
	Consecutive,
	Singleton,
	HashMap
};

class ComponentBase {
public:
	virtual Serializer& Serialize(Serializer& s);

	virtual std::string GetName() const = 0;
};
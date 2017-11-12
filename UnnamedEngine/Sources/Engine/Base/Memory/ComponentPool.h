#pragma once
#include "Engine/Base/Common/Common.h"

#include <utility>
#include <unordered_map>

#include "Engine/Base/Memory/ComponentPoolBase.h"

class ComponentPool : public ComponentPoolBase
{
public:
	ComponentPool(size_t blockSize) : ComponentPoolBase(blockSize) {}
	~ComponentPool() {} 
};
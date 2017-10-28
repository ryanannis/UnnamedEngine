#pragma once
#include "Engine/Base/Common/Common.h"

#include <utility>
#include <unordered_map>

#include "Engine/Base/Memory/ComponentPoolBase.h"

template <typename T>
class ComponentPool : public ComponentPoolBase
{
public:
	ComponentPool() : ComponentPoolBase(sizeof(t)) {}
	~ComponentPool() {} 
};
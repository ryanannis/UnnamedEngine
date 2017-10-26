#pragma once

#include "Engine/Base/Types/Component.h"

// let's try to keep this lightweight to compile time murder

class Component{
public:
	Component();
	virtual ~Component() = 0;
	virtual uint64_t GetComponentID() = 0;
private:
};
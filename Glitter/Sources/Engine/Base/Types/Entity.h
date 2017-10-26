#pragma once
#include <vector>

#include "Engine/Base/Types/Component.h"

class Entity
{
public:
	Entity();
	~Entity();
	std::vector<Component*> GetComponents();

private:
	uint32_t entityID;
	std::vector<Component*> mComponents;
};
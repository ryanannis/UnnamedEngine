#pragma once
#include <vector>

#include "Engine/Base/Types/Component.h"

class Entity
{
public:
	Entity();
	~Entity();
	inline uint32_t GetEntityId() const { return entityID; }

	std::vector<Component*> GetComponents();

private:
	uint32_t entityID;
	std::vector<Component*> mComponents;
};
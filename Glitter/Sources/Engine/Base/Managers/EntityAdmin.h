#pragma once

#include <vector>

#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Types/Entity.h"

class EntityAdmin
{
public:
	EntityAdmin();
	~EntityAdmin();

	void Update(const float delta);

private:
	std::vector<Entity> mEntities;

	// Disable copying
	EntityAdmin& operator=(const EntityAdmin&) = delete;
	EntityAdmin(const EntityAdmin&) = delete;
};
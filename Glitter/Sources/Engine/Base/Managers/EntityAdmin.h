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
};
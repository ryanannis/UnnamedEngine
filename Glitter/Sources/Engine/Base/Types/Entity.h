#pragma once
#include <vector>

#include "Engine/Base/Types/Component.h"

class EntityAdmin;


class Entity
{
public:
	Entity(EntityAdmin* admin, uint32_t id);

private:
	inline uint32_t GetEntityId() const { return(mEntityID); }

	EntityAdmin* mAdmin;
	uint32_t mEntityID;
	friend EntityAdmin;
};
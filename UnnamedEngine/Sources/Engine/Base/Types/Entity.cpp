#include "Entity.h"
#include "Engine/Base/Managers/EntityAdmin.h"

Entity::Entity(uint32_t id)
{
	mEntityID = static_cast<uint64_t>(id);
}

Entity::Entity(const Entity& entity) :
	mEntityID{entity.mEntityID}
{
}

void Entity::IncrementGeneration()
{
	mEntityID = mEntityID + (static_cast<uint64_t>(1) << 32);
}

void Entity::Increment()
{
	mEntityID = mEntityID + 1;
}

uint32_t Entity::GetGeneration() const
{
	return(mEntityID >> 32);
}

uint32_t Entity::GetIndex() const
{
	const uint64_t mask = 0x00000000ffffffff;
	return(mask & mEntityID);
}
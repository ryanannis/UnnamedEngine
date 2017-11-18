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
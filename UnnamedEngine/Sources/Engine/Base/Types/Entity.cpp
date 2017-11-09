#include "Entity.h"
#include "Engine/Base/Managers/EntityAdmin.h"

Entity::Entity(uint32_t id) :
	mEntityID(id)
{
}

Entity::Entity(const Entity& entity) :
	mEntityID{entity.mEntityID}
{
}
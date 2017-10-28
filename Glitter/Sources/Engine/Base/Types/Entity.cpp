#include "Entity.h"

Entity::Entity(EntityAdmin* admin, uint32_t id) :
	mAdmin(admin),
	mEntityID(id)
{
	assert(admin != nullptr);
}

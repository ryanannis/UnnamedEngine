#include "EntityAdmin.h"

static uint32_t entityCounter = 0;

EntityAdmin::EntityAdmin()
{
}

EntityAdmin::~EntityAdmin()
{
}

Entity EntityAdmin::CreateEntity()
{
	const uint32_t eid = entityCounter++;
	Entity entity{eid};
	mEntities.push_back(entity);
	return(entity);
}

void EntityAdmin::RegisterEntity(Entity* entity)
{
	const uint32_t entityId = entity->GetEntityId();
	mEntityMap.insert(std::pair<uint32_t, Entity*>(entityId, entity));
}
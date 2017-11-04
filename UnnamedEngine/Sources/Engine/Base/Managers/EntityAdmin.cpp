#include "EntityAdmin.h"

static uint32_t entityCounter = 0;

EntityAdmin::EntityAdmin()
{
}

EntityAdmin::~EntityAdmin()
{
}

uint32_t EntityAdmin::CreateEntity()
{
	const uint32_t nextId = entityCounter++;
	mEntities.emplace_back(nextId);
	return(nextId);
}

void EntityAdmin::RegisterEntity(Entity* entity)
{
	const uint32_t entityId = entity->GetEntityId();
	mEntityMap.insert(std::pair<uint32_t, Entity*>(entityId, entity));
}
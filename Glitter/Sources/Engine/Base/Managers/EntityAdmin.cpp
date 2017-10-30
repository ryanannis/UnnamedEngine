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
	mEntities.emplace_back(this, entityCounter++);
}

void EntityAdmin::RegisterEntity(const Entity* entity)
{
	const uint32_t entityId = entity->GetEntityId();
	mEntityMap.emplace(entityId, entity);
}

template<T>
Ptr<T> Entity::GetComponent(EntityAdmin* admin) const
{
	return(admin->GetComponent(this));
}
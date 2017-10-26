#include "EntityAdmin.h"

EntityAdmin::EntityAdmin()
{
}

EntityAdmin::~EntityAdmin()
{
}

void EntityAdmin::Update(const float delta)
{
	const auto systems = mSystemAdmin.GetSystems();
	for(const std::shared_ptr<System>& s : systems)
	{
		s->Update(delta);
	}
}

void EntityAdmin::RegisterEntity(const Entity const* entity)
{
	const uint32_t entityId = entity->GetEntityId();
	mEntityMap.emplace(entityId, entity);
}
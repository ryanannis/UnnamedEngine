#include "Entity.h"
#include "Engine/Base/Managers/EntityAdmin.h"

Entity::Entity(uint32_t id) :
	mEntityID(id)
{}

template <typename T>
Ptr<T> Entity::GetComponent(EntityAdmin* admin) const
{
	return(admin->GetComponent<T>(this));
}
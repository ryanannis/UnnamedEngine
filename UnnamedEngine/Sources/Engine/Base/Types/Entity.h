#pragma once
#include <vector>

#include "Engine/Base/Types/Component.h"

class EntityAdmin;

class Entity
{
public:
	Entity(const Entity& e);

	template <typename T>
	Ptr<T> GetComponent(EntityAdmin* admin) const;

private:
	Entity(uint32_t id);
	inline uint32_t GetEntityId() const { return(mEntityID); }

	uint32_t mEntityID;
	friend EntityAdmin;
};

template <typename T>
Ptr<T> Entity::GetComponent(EntityAdmin* admin) const
{
	return(admin->GetComponent<T>(this));
}
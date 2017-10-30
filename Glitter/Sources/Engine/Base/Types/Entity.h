#pragma once
#include <vector>

#include "Engine/Base/Types/Component.h"

class EntityAdmin;


class Entity
{
public:
	Entity(uint32_t id);

	template<T>
	Ptr<T> GetComponent(EntityAdmin* admin) const;

private:
	inline uint32_t GetEntityId() const { return(mEntityID); }

	uint32_t mEntityID;
	friend EntityAdmin;
};
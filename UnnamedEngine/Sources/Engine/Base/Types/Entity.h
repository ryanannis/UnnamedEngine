#pragma once
#include <vector>

#include "Engine/Base/Types/Component.h"

class EntityAdmin;

// A thin wrapper over a 64b uint, where the upper 32b are
// a generational ID (used for recycling memory) and the 
// lower 32b is an ID.
class Entity
{
public:
	Entity() {};
	Entity(const Entity& e);

	template <typename T>
	Ptr<T> GetComponent(EntityAdmin* admin) const;

	void IncrementGeneration();

	void Increment();

	uint32_t GetGeneration() const;

	uint32_t GetIndex() const;

private:
	// Inits an entity WITH A NEW GENERATION with the given ID
	Entity(uint32_t id);
	inline uint64_t GetEntityId() const { return(mEntityID); }

	uint64_t mEntityID;
	friend EntityAdmin;
};

template <typename T>
Ptr<T> Entity::GetComponent(EntityAdmin* admin) const
{
	return(admin->GetComponent<T>(this));
}
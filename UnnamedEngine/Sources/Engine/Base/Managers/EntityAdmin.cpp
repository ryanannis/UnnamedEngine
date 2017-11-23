#include "EntityAdmin.h"

EntityAdmin::EntityAdmin() :
	mEntityCounter(0)
{
}

EntityAdmin::~EntityAdmin()
{
}

Entity EntityAdmin::CreateEntity()
{
	// If we have a deleted entity - then recycle it
	if(mDeletedEntities.size() > 0)
	{
		Entity e = mDeletedEntities.back();
		mDeletedEntities.pop_back();
		e.IncrementGeneration();
		mGenerationTable[e.GetIndex()] = e.GetGeneration();
		mEntities[e.GetIndex()] = e;
		return(e);
	}

	// Otherwise create a fresh one
	const uint32_t eid = mEntityCounter++;
	Entity entity{eid};
	mGenerationTable.push_back(eid);
	mEntities.push_back(entity);
	return(entity);
}


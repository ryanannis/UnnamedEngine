#include "HashMapPool.h"

void* HashMapPool::GetComponent(uint32_t entityID)
{
	assert(BLOCK_SIZE > 0);
	auto ptrIterator = mHashMap.find(entityID);
	if(ptrIterator == mHashMap.end())
	{
		return(nullptr);
	}
	return(ptrIterator->second);
}

void* HashMapPool::AllocComponent(uint32_t entityID)
{
	assert(BLOCK_SIZE > 0);
	void* componentPtr = malloc(BLOCK_SIZE);
	mHashMap.insert(std::unordered_map<uint32_t, void*>::value_type(entityID, componentPtr));
	return(componentPtr);
}

void HashMapPool::DeleteComponent(uint32_t entityID)
{
	assert(BLOCK_SIZE > 0);
	auto ptrIterator = mHashMap.find(entityID);
	assert(ptrIterator != mHashMap.end()); // INVALID
	free(ptrIterator->second);
}
#pragma once
#include "Engine/Base/Common/Common.h"

#include <utility>
#include <unordered_map>

/* 
 * A fixed block-size component pool (currently does nothing special).  
 * The pool is created via a ComponentPool<T>, which provides size
 * information along with typing information for garbage collection
 * and deletion.
 */
class ComponentPoolBase : public NonCopyable
{
public:
	explicit ComponentPoolBase(size_t BLOCK_SIZE) : BLOCK_SIZE(BLOCK_SIZE) {};
	~ComponentPoolBase() = 0;

	const size_t BLOCK_SIZE = 0;

	inline void* GetComponent(uint32_t entityID)
	{
		assert(BLOCK_SIZE > 0);
		auto ptrIterator = mREMOVEMEMAP.find(entityID);
		assert(ptrIterator != mREMOVEMEMAP.end()); // INVALID 
		return(ptrIterator->second);
	}

	inline void* AllocComponent(uint32_t entityID)
	{
		assert(BLOCK_SIZE > 0);
		void* componentPtr = malloc(BLOCK_SIZE);
		mREMOVEMEMAP.insert(std::unordered_map<uint32_t, void*>::value_type(entityID, componentPtr));
		return(componentPtr);
	}

	inline void DeleteComponent(uint32_t entityID)
	{
		assert(BLOCK_SIZE > 0);
		auto ptrIterator = mREMOVEMEMAP.find(entityID);
		assert(ptrIterator != mREMOVEMEMAP.end()); // INVALID
		free(ptrIterator->second);
	}

private:
	std::unordered_map<uint32_t, void*> mREMOVEMEMAP;
};
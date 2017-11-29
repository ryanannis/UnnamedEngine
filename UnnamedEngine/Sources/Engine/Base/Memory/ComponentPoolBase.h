#pragma once
#include "Engine/Base/Common/Common.h"

/* 
 * A fixed block-size component pool (currently does nothing special).  
 * The pool is created via a ComponentPool<T>, which provides size
 * information along with typing information for garbage collection
 * and deletion.
 */
class ComponentPoolBase : public NonCopyable
{
public:
	ComponentPoolBase() = default;
	virtual void* GetComponent(uint32_t entityID) = 0;
	virtual void* AllocComponent(uint32_t entityID) = 0;
	virtual void DeleteComponent(uint32_t entityID) = 0;
	// Component ptrs are not guaranteed between frames so this gives time 
	// to collate memory / garbage collect etc.
	virtual void OnBetweenFrames() {};
};
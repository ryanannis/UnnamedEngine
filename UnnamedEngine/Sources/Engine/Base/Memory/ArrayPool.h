#pragma once
#include "Engine/Base/Common/Common.h"

#include <utility>
#include <unordered_map>

#include "Engine/Base/Memory/ComponentPoolBase.h"

class ArrayPool : public ComponentPoolBase
{
public:
	ArrayPool(size_t blockSize);
	virtual void* GetComponent(uint32_t entityID) override;
	virtual void* AllocComponent(uint32_t entityID) override;
	virtual void DeleteComponent(uint32_t entityID) override;

private:
	void DoublePoolSize();
	size_t mCapacity;
	size_t mBlockSize;
	void* mStart;
	void* mEnd;
};
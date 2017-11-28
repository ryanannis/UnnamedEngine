#include "ArrayPool.h"

ArrayPool::ArrayPool(size_t blockSize) :
	mBlockSize(blockSize)
{
	mCapacity = 48;
	mStart = malloc(mBlockSize * mCapacity);
	mEnd = static_cast<char*>(mStart) + mBlockSize * mCapacity;
}

void* ArrayPool::GetComponent(uint32_t entityID)
{
	void* ptr = static_cast<char*>(mStart) + mBlockSize * entityID;
	assert(ptr < mEnd);
	return(ptr);
}

/* Fundamentally the same as GetComponent except is also allowed to resize the mem pool*/
void* ArrayPool::AllocComponent(uint32_t entityID)
{
	void* add = static_cast<char*>(mStart) + mBlockSize * entityID;
	if(add >= mEnd)
	{
		DoublePoolSize();
		return(AllocComponent(entityID));
	}
	return(add);
}

void ArrayPool::DeleteComponent(uint32_t entityID)
{
	//todo:  arraypool should only be able to resize, not delete?  unless we want components with heap alloced stuff (ew)
}

void ArrayPool::DoublePoolSize()
{
	size_t newCapacity = mCapacity * 2;
	void *newStart = malloc(mBlockSize * mCapacity);
	void *newEnd = static_cast<char*>(newStart) + mBlockSize * mCapacity;
	std::memcpy(newStart, mStart, mCapacity);
	free(mStart);
	mStart = newStart;
	mEnd = newEnd;
	mCapacity = newCapacity;
}

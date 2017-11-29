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

	if(entityID >= mCapacity)
	{
		size_t ctr = mCapacity;
		// Then component is in uncollated memory
		// todo:  do based on powers of 2
		for(size_t i = 0; i < mUncollatedMemory.size(); i++)
		{
			if(entityID < ctr + mUncollatedMemory[i].second)
			{
				void* ptr = static_cast<char*>(mUncollatedMemory[i].first) + mBlockSize * (entityID - ctr);
				return(ptr);
			}
			ctr += mUncollatedMemory[i].second;
		}
		assert(false); // invalid access!
	}

	return(ptr);
}

/* Fundamentally the same as GetComponent except is also allowed to resize the mem pool*/
void* ArrayPool::AllocComponent(uint32_t entityID)
{
	void* add = static_cast<char*>(mStart) + mBlockSize * entityID;
	if(entityID >= mCapacity)
	{
		size_t ctr = mCapacity;
		for(size_t i = 0; i < mUncollatedMemory.size(); i++)
		{
			if(entityID < ctr + mUncollatedMemory[i].second)
			{
				void* ptr = static_cast<char*>(mUncollatedMemory[i].first) + mBlockSize * (entityID - ctr);
				return(ptr);
			}
			ctr += mUncollatedMemory[i].second;
		}
		// we're out of space - time to create another uncollated block
		mUncollatedMemory.push_back(std::pair<void*, int>(malloc(mBlockSize * ctr), ctr));
		return(AllocComponent(entityID));
	}
	return(add);
}

void ArrayPool::DeleteComponent(uint32_t entityID)
{
	//todo:  arraypool should only be able to resize, not delete?  unless we want components with heap alloced stuff (ew)
}

void ArrayPool::OnBetweenFrames()
{
	CollateMemory();
}

void ArrayPool::CollateMemory()
{
	//todo
}
#include "FatComponent.h"

FatComponent::FatComponent()
{
	for(size_t i = 1; i < 50; i++)
	{
		mFatness[i] = mFatness[i - 1] << 2 * 5 + 6;
	}
}


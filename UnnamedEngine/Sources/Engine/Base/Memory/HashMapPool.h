#pragma once
#include "Engine/Base/Common/Common.h"

#include <utility>
#include <unordered_map>

#include "Engine/Base/Memory/ComponentPoolBase.h"

class HashMapPool : public ComponentPoolBase
{
public:
	explicit HashMapPool(size_t BLOCK_SIZE) :
		BLOCK_SIZE(BLOCK_SIZE) 
	{};

	~HashMapPool() {}; // todo: implement this

	const size_t BLOCK_SIZE = 0;

	virtual void* GetComponent(uint32_t entityID) override;
	virtual void* AllocComponent(uint32_t entityID) override;
	virtual void DeleteComponent(uint32_t entityID) override;

private:
	std::unordered_map<uint32_t, void*> mHashMap;
};
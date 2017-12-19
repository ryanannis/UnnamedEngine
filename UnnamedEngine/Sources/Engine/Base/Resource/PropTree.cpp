#include "PropTree.h"
#include "Engine/Base/Resource/PropParser.h"

// For 'URI'
#include "Engine/Base/Resource/ResourceManager.h"

#include <sstream>

void PropTree::AddLeaf(std::string name, const std::string& leaf)
{
	assert(leaves.find(name) == leaves.end());
	leaves.emplace(
		name, 
		static_cast<PropTreeLeafBase*>(new PropTreeLeaf<std::string>(leaf))
	);
}

void PropTree::AddLeaf(std::string name, float leaf)
{
	assert(leaves.find(name) == leaves.end());
	leaves.emplace(
		name,
		static_cast<PropTreeLeafBase*>(new PropTreeLeaf<float>(leaf))
	);
}

void PropTree::AddLeaf(std::string name, int leaf)
{
	assert(leaves.find(name) == leaves.end());
	leaves.emplace(
		name,
		static_cast<PropTreeLeafBase*>(new PropTreeLeaf<int>(leaf))
	);
}

void PropTree::AddLeaf(std::string name, const URI& leaf)
{
	assert(leaves.find(name) == leaves.end());
	leaves.emplace(
		name,
		static_cast<PropTreeLeafBase*>(new PropTreeLeaf<URI>(leaf))
	);
}

void PropTree::AddLeaf(std::string name, const Vector3f& leaf)
{
	assert(leaves.find(name) == leaves.end());
	leaves.emplace(
		name,
		static_cast<PropTreeLeafBase*>(new PropTreeLeaf<Vector3f>(leaf))
	);
}

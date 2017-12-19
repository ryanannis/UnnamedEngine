#include "LevelObject.h"
#include "Engine/Base/Resource/ResourceManager.h"

LevelObject::LevelObject(const PropTree& tree)
{
	// Initialize
	auto file = tree.GetLeaf<URI>("Resource");
	auto position = tree.GetLeaf<Vector3f>("Position");
	auto rotation = tree.GetLeaf<Vector3f>("Rotation");

	assert(file);
	assert(position);
	assert(rotation);

	auto parsedFile = file->Get();
	auto parsedPosition = position->Get();
	auto parsedRotation = rotation->Get();

	//todo : Have ResourceType return a URI
	mResource = ResourceType<EntityResource>(parsedFile);
	mPosition = parsedPosition;
	parsedRotation = parsedRotation;
}

const ResourceType<EntityResource>& LevelObject::GetResource() const
{
	return(mResource);
}

Vector3f LevelObject::GetPosition() const
{
	return(mPosition);
}

Vector3f LevelObject::GetEulerRotation() const
{
	return(mRotation);
}

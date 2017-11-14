#include "LevelObject.h"
#include "Engine/Base/Resource/ResourceManager.h"

LevelObject::LevelObject(const PropTree& tree)
{
	// Initialize
	auto file = tree.leaves.find("Resource");
	auto position = tree.leaves.find("Position");
	auto rotation = tree.leaves.find("Resource");

	assert(file != tree.leaves.end());
	assert(position != tree.leaves.end());
	assert(rotation != tree.leaves.end());

	auto parsedFile = file->second.GetAsURI();
	auto parsedPosition = position->second.GetAsVector();
	auto parsedRotation = rotation->second.GetAsVector();

	//todo : Have ResourceType return a URI
	mResource = ResourceType<EntityResource>(parsedFile->GetFilePath());
	mPosition = *parsedPosition;
	parsedRotation = *parsedRotation;
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

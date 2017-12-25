#include "ModelResource.h"

#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <glm/glm.hpp>
#include "Engine/Base/Resource/MaterialResource.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/ResourceManager.h"


ModelResource::ModelResource(URI uri) : 
	Resource(uri),
	mReady(false)
{
}

bool ModelResource::IsReady() const
{
	return(mReady);
}

const std::vector<Mesh>& ModelResource::GetMeshes() const
{
	return(mMeshes);
}

void ModelResource::Load(Ptr<ResourceManager> manager)
{
}
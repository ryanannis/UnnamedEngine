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
	// We don't have a format yet so we're just using assimp
	// In the future, that should be moved to an asset processing
	// module - and have our own custom data format
	// Load a Model from File
	Assimp::Importer loader;
	aiScene const *scene = loader.ReadFile(
		GetURI().GetFilePath(),
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_OptimizeGraph |
		aiProcess_OptimizeMeshes
	);

	// Walk the Tree of Scene Nodes
	if(!scene)
	{
		fprintf(stderr, "%s\n", loader.GetErrorString());
	}
	else
	{
		ProcessAssimpNode(manager, scene->mRootNode, scene);
		mReady = true;
	}
}
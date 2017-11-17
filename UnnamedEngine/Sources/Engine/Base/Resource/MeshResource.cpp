#include "MeshResource.h"

#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <glm/glm.hpp>

MeshResource::MeshResource(std::string URI) : 
	Resource(URI),
	mReady(false)
{
}

bool MeshResource::IsReady() const
{
	return(mReady);
}

void MeshResource::Load(Ptr<ResourceManager> manager)
{
	// We don't have a format yet so we're just using assimp
	// In the future, that should be moved to an asset processing
	// module - and have our own custom data format
	// Load a Model from File
	Assimp::Importer loader;
	aiScene const *scene = loader.ReadFile(
		CONTENT_DIR + GetURI(),
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_OptimizeGraph |
		aiProcess_FlipUVs
	);

	// Walk the Tree of Scene Nodes
	if(!scene)
	{
		fprintf(stderr, "%s\n", loader.GetErrorString());
	}
	else
	{
		if(scene->mRootNode && scene->mRootNode->mNumMeshes > 0)
		{
			auto mesh = scene->mMeshes[scene->mRootNode->mMeshes[0]];
			Parse
			(
				mesh,
				mIndices,
				mUVs,
				mVertices,
				mNormals
			);
		}
		mReady = true;
	}
}

void MeshResource::Parse(
	aiMesh const* mesh,
	std::vector<uint32_t>& indices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec3>& normals
) const
{
	// Create Vertex Data from Mesh Node
	for(size_t i = 0; i < mesh->mNumVertices; i++)
	{
		if(mesh->mTextureCoords[0])
		{
			
		}
		vertices.push_back(glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z));
		normals.push_back(glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z));
	}

	for(size_t i = 0; i < mesh->mNumFaces; i++)
	{
		for(size_t j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}
}
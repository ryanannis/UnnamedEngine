#include "ModelResource.h"

#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <glm/glm.hpp>

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
		aiProcess_FlipUVs |
		aiProcess_OptimizeMeshes
	);

	// Walk the Tree of Scene Nodes
	if(!scene)
	{
		fprintf(stderr, "%s\n", loader.GetErrorString());
		assert(false);
	}
	else
	{
		ProcessAssimpNode(scene->mRootNode, scene);
		mReady = true;
	}
}

void ModelResource::ProcessAssimpNode(const aiNode* node, const aiScene* scene)
{
	// no node - this should only happen at root
	if(!node)
	{
		return;
	}

	// parse meshes
	for(size_t i = 0; i < node->mNumMeshes; i++)
	{
		auto mesh = scene->mMeshes[scene->mRootNode->mMeshes[i]];
		Parse(mesh);
	}

	// recursivelyparse children
	for(size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessAssimpNode(node->mChildren[i], scene);
	}
}

void ModelResource::Parse(aiMesh const* mesh)
{
	std::vector<uint32_t> indices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;

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

	Mesh newMesh;
	newMesh.mIndices = indices;
	newMesh.mNormals = normals;
	newMesh.mVertices = vertices;
	newMesh.mUVs = uvs;

	mMeshes.push_back(newMesh);
}
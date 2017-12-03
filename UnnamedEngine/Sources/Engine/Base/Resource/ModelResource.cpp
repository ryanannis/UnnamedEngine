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
		aiProcess_FlipUVs |
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


void ModelResource::ProcessAssimpNode(Ptr<ResourceManager> manager, const aiNode* node, const aiScene* scene)
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
		Parse(manager, mesh, scene);
	}

	// recursivelyparse children
	for(size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessAssimpNode(manager, node->mChildren[i], scene);
	}
}

void ModelResource::Parse(Ptr<ResourceManager> manager, aiMesh const* mesh, const aiScene* scene)
{
	std::vector<uint32_t> indices;
	std::vector<float> vectors;

	// Create Vertex Data from Mesh Node
	for(size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vectors.push_back(mesh->mVertices[i].x);
		vectors.push_back(mesh->mVertices[i].y);
		vectors.push_back(mesh->mVertices[i].z);
		vectors.push_back(mesh->mNormals[i].x);
		vectors.push_back(mesh->mNormals[i].y);
		vectors.push_back(mesh->mNormals[i].z);
		if(mesh->mTextureCoords[0])
		{
			const auto textureCoords = mesh->mTextureCoords[0];
			vectors.push_back(textureCoords[i].x);
			vectors.push_back(textureCoords[i].y);
		}
		else
		{
			vectors.push_back(0.f);
			vectors.push_back(0.f);
		}
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
	newMesh.mVectors = vectors;

	// Load textures
	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		newMesh.mDiffuseTextures = LoadMaterials(manager, material, aiTextureType_DIFFUSE);
		newMesh.mSpecularTextures = LoadMaterials(manager, material, aiTextureType_SPECULAR);
	}

	mMeshes.push_back(newMesh);
}

std::vector<ResourceType<MaterialResource>> ModelResource::LoadMaterials
(
	Ptr<ResourceManager> manager,
	aiMaterial const* material,
	aiTextureType type
)
{
	std::vector<ResourceType<MaterialResource>> resources;

	for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		// Assimp returns path relative to .mat
		auto path = GetURI().GetPathFromRoot() + str.C_Str();

		resources.push_back(ResourceType<MaterialResource>(path));
	}
	return(resources);
}


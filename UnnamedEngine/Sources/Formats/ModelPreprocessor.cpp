#include "ModelPreprocessor.h"

#include <fstream>

#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>

void MeshPreprocessor::PreprocessMesh(URI i)
{
	MeshData m = ParseMesh(i);

	std::string filename = i.GetCachePath();
	
	// todo:  might need stronger binary serialization abstraction for doing networking
	{
		std::ofstream ostrm(filename, std::ios::binary);
		ostrm.write(reinterpret_cast<const char*>(&MESH_FORMAT_HEADER), sizeof(MESH_FORMAT_HEADER));

		const size_t numSubmeshes = m.numSubmeshes;
		ostrm.write(reinterpret_cast<const char*>(&numSubmeshes), sizeof(numSubmeshes));
		for(size_t i = 0; i < numSubmeshes; i++)
		{
			const SubmeshData& submesh = m.submeshes[i];
			
			assert(submesh.indices);
			assert(submesh.interleavedData);

			ostrm.write(reinterpret_cast<const char*>(&submesh.properties), sizeof(submesh.properties));
			ostrm.write(reinterpret_cast<const char*>(&submesh.numVertices), sizeof(submesh.numVertices));
			ostrm.write(reinterpret_cast<const char*>(&submesh.indices), sizeof(submesh.numIndices));
			ostrm.write(reinterpret_cast<const char*>(&submesh.numUVs), sizeof(submesh.numUVs));
			
			const uint32_t interleavedVerticeDataSize = submesh.GetInterleavedSize();
			
			const auto fuck = submesh.interleavedData;

			ostrm.write(reinterpret_cast<const char*>(submesh.interleavedData), interleavedVerticeDataSize * submesh.numVertices);
			ostrm.write(reinterpret_cast<const char*>(submesh.indices), sizeof(uint32_t) * submesh.numIndices);
			
			// todo:  write material
		}

		ostrm.close();
	}

	m.Release();
}

MeshData MeshPreprocessor::ParseMesh(URI i)
{
	Assimp::Importer loader;
	aiScene const *scene = loader.ReadFile(
		i.GetFilePath(),
		aiProcessPreset_TargetRealtime_MaxQuality |
		aiProcess_OptimizeGraph |
		aiProcess_OptimizeMeshes
	);

	if(!scene)
	{
		fprintf(stderr, "%s\n", loader.GetErrorString());
		assert(false);
	}

	ProcessAssimpNode(scene->mRootNode, scene);
	const size_t numSubmeshes = mSubmeshes.size();
	SubmeshData* submeshHeapData = static_cast<SubmeshData*>(malloc(sizeof(SubmeshData) * numSubmeshes));
	std::copy(mSubmeshes.begin(), mSubmeshes.end(), &submeshHeapData[0]);

	MeshData m;
	m.numSubmeshes = numSubmeshes;
	m.submeshes = submeshHeapData;
	return(m);
}

void MeshPreprocessor::ProcessAssimpNode(const aiNode* node, const aiScene* scene)
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
		mSubmeshes.push_back(Parse(mesh, scene));
	}

	// recursivelyparse children
	for(size_t i = 0; i < node->mNumChildren; i++)
	{
		ProcessAssimpNode(node->mChildren[i], scene);
	}
}

SubmeshData MeshPreprocessor::Parse(aiMesh const* mesh, const aiScene* scene)
{
	std::vector<uint32_t> indices;
	std::vector<float> vectors;

	// Create Vertex Data from Mesh Node
	const auto numUvs = mesh->GetNumUVChannels();
	const auto hasNormals = true;
	const auto hasMaterial = false;

	const auto numVertices = mesh->mNumVertices;
	for(size_t i = 0; i < numVertices; i++)
	{
		vectors.push_back(mesh->mVertices[i].x);
		vectors.push_back(mesh->mVertices[i].y);
		vectors.push_back(mesh->mVertices[i].z);

		vectors.push_back(mesh->mNormals[i].x);
		vectors.push_back(mesh->mNormals[i].y);
		vectors.push_back(mesh->mNormals[i].z);
		for(size_t i = 0; i < numUvs; i++)
		{
			const auto textureCoords = mesh->mTextureCoords[i];
			vectors.push_back(textureCoords[i].x);
			vectors.push_back(textureCoords[i].y);
		}
	}

	for(size_t i = 0; i < mesh->mNumFaces; i++)
	{
		for(size_t j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	const size_t numVecs = vectors.size();
	const size_t numIndices = indices.size();
	float* heapInterleavedData = static_cast<float*>(malloc(sizeof(float) * numVecs));
	uint32_t* heapIndiceData = static_cast<uint32_t*>(malloc(sizeof(uint32_t) * numIndices));

	std::copy(vectors.begin(), vectors.end(), &heapInterleavedData[0]);
	std::copy(indices.begin(), indices.end(), &heapIndiceData[0]);

	SubmeshData m;
	if(hasMaterial)
	{
		m.properties |= HAS_MATERIAL_BYTE_OFFSET;
	}

	if(hasNormals)
	{
		m.properties |= HAS_NORMALS_BYTE_OFFSET;
	}

	m.interleavedData = heapInterleavedData;
	m.indices = heapIndiceData;
	m.numVertices = numVertices;
	m.numIndices = numIndices;
	m.numUVs = numUvs;

	for(size_t i = 0; i < mesh->mNumFaces; i++)
	{
		for(size_t j = 0; j < mesh->mFaces[i].mNumIndices; j++)
		{
			indices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	return(m);
	/*
	// Load textures
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
	}

	mMeshes.push_back(newMesh);
	*/
}

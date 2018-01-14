#include "ModelLoader.h"

#include "Engine/Base/Resource/URI.h"
#include <fstream>
#include <streambuf>

#ifdef _DEBUG
#include "Formats/ModelPreprocessor.h"
#endif

const MeshData PrivateLoadModel(URI uri)
{
	// todo:  this has... questionable... performance
	const auto fileUrl = uri.GetCachePath();
	std::ifstream t(fileUrl, std::ios::binary);

	// Make sure we are actually deserializing a mesh
	uint32_t header;
	t.read(reinterpret_cast<char*>(&header), sizeof(header));
	assert(header == MESH_FORMAT_HEADER);

	MeshData d;
	t.read(reinterpret_cast<char*>(&d.numSubmeshes), sizeof(uint32_t));

	// Deserialize submeshes
	SubmeshData* submeshData = static_cast<SubmeshData*>(malloc(d.numSubmeshes * sizeof(SubmeshData)));
	d.submeshes = submeshData;

	for(size_t i = 0; i < d.numSubmeshes; i++)
	{
		SubmeshData& currentSubmesh = submeshData[i];
		t.read(reinterpret_cast<char*>(&submeshData->properties), sizeof(uint32_t));
		t.read(reinterpret_cast<char*>(&submeshData->numVertices), sizeof(uint32_t));
		t.read(reinterpret_cast<char*>(&submeshData->numUVs), sizeof(uint32_t));
		t.read(reinterpret_cast<char*>(&submeshData->numIndices), sizeof(uint32_t));
		const auto interleavedDataSize = submeshData->GetInterleavedSize() * submeshData->numVertices;
		const auto indiceDataSize = submeshData->numIndices * sizeof(uint32_t);

		float* interleavedData = static_cast<float*>(malloc(interleavedDataSize));
		uint32_t* indiceData = static_cast<uint32_t*>(malloc(indiceDataSize));

		// Possible to be 0
		if (interleavedData)
		{
			t.read(reinterpret_cast<char*>(interleavedData), interleavedDataSize);
		}
		if (indiceData)
		{
			t.read(reinterpret_cast<char*>(indiceData), indiceDataSize);
		}

		currentSubmesh.interleavedData = interleavedData;
		currentSubmesh.indices = indiceData;
	}

	return(d);
}

const bool DoesFileExist(URI uri)
{
	auto stream = std::ifstream(uri.GetCachePath());
	return(stream.good());
}

const MeshData ModelLoader::LoadModel(URI uri)
{
	const auto fileExists = DoesFileExist(uri);
#ifdef _DEBUG
	if(!fileExists)
	{
		MeshPreprocessor p;
		p.PreprocessMesh(uri);
	}
#endif // DEBUG
	return(PrivateLoadModel(uri));
}
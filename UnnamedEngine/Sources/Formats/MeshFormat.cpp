#include "MeshFormat.h"

SubmeshData::SubmeshData() :
	properties{0},
	numVertices{0},
	numUVs{0},
	numIndices{0},
	interleavedData(nullptr),
	indices{nullptr},
	materialRef{nullptr}
{
}

void SubmeshData::Release()
{
	free(interleavedData);
	free(indices);
	free(materialRef);
}

void MeshData::Release()
{
	for(size_t i = 0; i < numSubmeshes; i++)
	{
		submeshes[i].Release();
	}
	free(submeshes);
}

uint32_t SubmeshData::GetInterleavedSize() const
{
	uint32_t interleavedVerticeDataSize = 0;

	// Vertex data is xyz single precision floats
	interleavedVerticeDataSize += 3 * sizeof(float);
	if(properties & HAS_NORMALS_BYTE_OFFSET)
	{
		// Normal data is xyz single precision floats
		interleavedVerticeDataSize += 3 * sizeof(float);
	}
	// UV's are uv single precision float
	interleavedVerticeDataSize += 2 * sizeof(float) * numUVs;

	return(interleavedVerticeDataSize);
}
#pragma once
#include "Engine/Base/Common/Common.h"

const static size_t HAS_MATERIAL_BYTE_OFFSET = 0;
const static size_t HAS_NORMALS_BYTE_OFFSET = 1;

struct SubmeshData
{
	SubmeshData();
	uint32_t properties;
	uint32_t numVertices;
	uint32_t numUVs;
	uint32_t numIndices;
	float* interleavedData;
	uint32_t* indices;
	char* materialRef;

	void Release();
	uint32_t GetInterleavedSize() const;
};

struct MeshData
{
	uint32_t numSubmeshes;
	SubmeshData* submeshes;

	void Release();
};
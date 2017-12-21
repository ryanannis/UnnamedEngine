#pragma once

struct SubmeshData
{
	bool hasMaterial;
	bool hasNormals;
	// Empty bit
	// Empty bit
	uint32_t numVertices;
	uint32_t numUVs;
	uint32_t numIndices;
	float* interleavedData;
	uint32_t* indices;
	char* materialRef;

	void Release();
};

struct MeshData
{
	uint32_t numSubmeshes;
	SubmeshData* submeshes;

	void Release();
};
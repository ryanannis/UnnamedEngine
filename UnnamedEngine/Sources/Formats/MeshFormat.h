#pragma once

struct SubmeshData
{
	char* materialRef;
	bool hasUVs;
	// Empty bit
	// Empty bit
	// Empty bit
	uint32_t numVertices;
	uint32_t numIndices;
	uint32_t* vertices;
	uint32_t* indices;
	uint32_t* uvs;
};

struct MeshData
{
	uint32_t numSubmeshes;
	SubmeshData* submeshes;
};
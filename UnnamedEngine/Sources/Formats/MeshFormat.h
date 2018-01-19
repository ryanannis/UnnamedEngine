#pragma once
#include "Engine/Base/Common/Common.h"

/* SubmeshData bitfield properties. */

/* True if materialRef is non-null. */
const static size_t HAS_MATERIAL_BYTE_OFFSET = 1 << 0;
/* True if interleavedData contains normal data. */
const static size_t HAS_NORMALS_BYTE_OFFSET = 1 << 1;

/* An file format identifier printed as the first line of the header.  */
const static uint32_t MESH_FORMAT_HEADER = 10000;

struct InterleavedOffset
{
	char byteOffset;
	char byteSize;
};

/*
* SubmeshData is stored interleaved, for direct use in the graphics driver.
* SubmeshData always contains vertex and indices information, and optionally 
* contains UV and normal data.
*
* Interleaved data is contained in the field interleavedData with the following
* format:
*
* 3*float				Vertex data
* 3*float				Normal data (OPTIONAL, enabled with HAS_NORMALS_BYTE_OFFSET)
* 2*float*numUVs		UV data. The UV index corresponds to a texture index in materialRef.
*/
struct SubmeshData
{
	SubmeshData();
	uint32_t properties;
	uint32_t numVertices;
	uint32_t numUVs;
	uint32_t numIndices;
	
	// See class description for description of offsets.
	// As this a float array, there is no alignment issues.
	float* interleavedData;
	uint32_t* indices;
	char* materialRef;

	void Release();

	uint32_t GetNumberLocations() const;
	uint32_t GetInterleavedSize() const;
	uint32_t GetTotalBufferSize() const;
};

/*
 * Each MeshData contains 0 to n SubmeshData, which contain the vertex data for
 * each seperate renderable part of a larger mesh.
 *
 */
struct MeshData
{
	uint32_t numSubmeshes;
	SubmeshData* submeshes;

	void Release();
};
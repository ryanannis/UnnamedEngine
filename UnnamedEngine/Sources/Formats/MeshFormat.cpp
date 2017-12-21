#include "MeshFormat.h"

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
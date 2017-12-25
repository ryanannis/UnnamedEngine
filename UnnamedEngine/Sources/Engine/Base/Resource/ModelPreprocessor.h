#pragma once

#include "Engine/Base/Resource/URI.h"
#include "Formats/MeshFormat.h"

struct aiMesh;
struct aiNode;
struct aiScene;
struct aiMaterial;
enum aiTextureType;
class MaterialResource;

class MeshPreprocessor
{
public:
	void PreprocessMesh(URI i);

private:
	MeshData ParseMesh(URI i);
	void ProcessAssimpNode(const aiNode* node, const aiScene* scene);
	SubmeshData Parse(aiMesh const* mesh, const aiScene* scene);
	std::vector<SubmeshData> mSubmeshes;
};
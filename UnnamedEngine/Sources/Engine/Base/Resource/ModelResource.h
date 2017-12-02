#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/Resource.h"

#include <vector>
#include <map>

struct aiMesh;
struct aiNode;
struct aiScene;
struct aiMaterial;
enum aiTextureType;
class MaterialResource;

struct Mesh
{
	std::vector<uint32_t> mIndices;
	std::vector<glm::vec2> mUVs;
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
	std::vector<std::weak_ptr<MaterialResource>> mDiffuseTextures;
	std::vector<std::weak_ptr<MaterialResource>> mSpecularTextures;
};

class ModelResource : public Resource
{
public:
	ModelResource(URI URI);
	virtual bool IsReady() const override;
	virtual void Load(Ptr<ResourceManager> manager) override;
	const std::vector<Mesh>& GetMeshes() const;

private:
	void ProcessAssimpNode(Ptr<ResourceManager> manager, const aiNode* node, const aiScene* scene);
	void Parse(Ptr<ResourceManager> manager, aiMesh const* mesh, const aiScene* scene);
	std::vector<std::weak_ptr<MaterialResource>> LoadMaterials(Ptr<ResourceManager> manager, aiMaterial const* material, aiTextureType type);

	std::vector<Mesh> mMeshes;
	bool mReady;
};
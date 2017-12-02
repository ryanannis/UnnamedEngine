#pragma once
#include "Engine/Base/Common/Common.h"

#include <vector>
#include <map>

#include "Engine/Base/Resource/Resource.h"

struct aiMesh;
struct aiNode;
struct aiScene;

struct Mesh
{
	std::vector<uint32_t> mIndices;
	std::vector<glm::vec2> mUVs;
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
};

class ModelResource : public Resource
{
public:
	ModelResource(URI URI);
	virtual bool IsReady() const override;
	virtual void Load(Ptr<ResourceManager> manager) override;
	const std::vector<Mesh>& GetMeshes() const;

private:
	void ProcessAssimpNode(const aiNode* node, const aiScene* scene);
	void Parse(aiMesh const* scene);

	std::vector<Mesh> mMeshes;

	bool mReady;
};
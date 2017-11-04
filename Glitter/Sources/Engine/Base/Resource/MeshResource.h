#pragma once
#include "Engine/Base/Common/Common.h"

#include <vector>
#include <map>

#include "Engine/Base/Resource/Resource.h"

struct aiMesh;


class MeshResource : Resource
{
public:
	MeshResource(std::string URI);
	virtual bool IsReady() const override;
	virtual void Load() override;

private:
	void Parse(
		aiMesh const* scene,
		std::vector<uint32_t>& indices,
		std::vector<glm::vec3> vertices,
		std::vector<glm::vec3> normals
	) const;

	std::vector<uint32_t> mIndices;
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;

	bool mReady;
};
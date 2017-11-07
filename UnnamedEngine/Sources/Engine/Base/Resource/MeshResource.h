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

	inline const std::vector<uint32_t>& GetIndices() const { return(mIndices); }
	inline const std::vector<glm::vec2>& GetUVs() const { return(mUVs); }
	inline const std::vector<glm::vec3>& GetVertices() const { return(mVertices); }
	inline const std::vector<glm::vec3>& GetNormals() const { return(mNormals); }

private:
	void Parse(
		aiMesh const* scene,
		std::vector<uint32_t>& indices,
		std::vector<glm::vec2>& uvs,
		std::vector<glm::vec3>& vertices,
		std::vector<glm::vec3>& normals
	) const;

	std::vector<uint32_t> mIndices;
	std::vector<glm::vec2> mUVs;
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;

	bool mReady;
};
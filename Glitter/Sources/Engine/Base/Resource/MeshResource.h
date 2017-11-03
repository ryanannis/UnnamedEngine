#pragma once
#include "Engine/Base/Common/Common.h"

#include <vector>
#include <map>

#include "Engine/Base/Resource/Resource.h"


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
		std::vector<glm::vec3> normals,
		std::map<GLuint, std::string> textures
	) const;

	std::vector<uint32_t> mIndices;
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
	std::map<GLuint, std::string> mTextures;

	bool mReady;
};
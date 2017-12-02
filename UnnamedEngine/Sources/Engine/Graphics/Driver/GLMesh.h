#pragma once
#include "Engine/Base/Common/Common.h"

#include <memory>
#include <glad/glad.h>

class ModelResource;

struct GLSubmesh
{
	GLuint verticesbuffer;
	GLuint indicesBuffer;
	size_t numIndices;
};

class GLMesh
{
public:
	GLMesh(const std::weak_ptr<ModelResource>& resource);
	void Free();
	const std::vector<GLSubmesh>& GetSubmeshes() const;


private:
	std::vector<GLSubmesh> mSubmeshes;
};
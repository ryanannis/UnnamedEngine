#pragma once
#include "Engine/Base/Common/Common.h"
#include <glad/glad.h>
#include <memory>

class ShaderResource;

class GLMesh
{
public:
	GLMesh();
	GLMesh(
		const std::vector<float>& vertices, 
		const std::vector<GLuint>& indices
	);
	void Bind() const;
	void Unbind() const;
	void Free();
	size_t GetNumIndices() const;
	GLuint GetHandle() const;

private:
	GLuint mVertices;
	GLuint mIndices;
	size_t mNumIndices;
};
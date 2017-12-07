#include "GLMesh.h"

#include <memory>
#include <glad/glad.h>

#include "Engine/Graphics/Driver/GLCommon.h"

GLMesh::GLMesh() : 
	mVertices(0),
	mIndices(0)
{}

GLMesh::GLMesh(
	const std::vector<float>& vertices,
	const std::vector<GLuint>& indices
)
{
	//Bind VBO of interleaved vertices
	glGenBuffers(1, &mVertices);
	glBindBuffer(GL_ARRAY_BUFFER, mVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	// Bind EBO
	glGenBuffers(1, &mIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

	mNumIndices = indices.size();
}

void GLMesh::Free()
{
	glDeleteBuffers(1, &mVertices);
	glDeleteBuffers(1, &mIndices);
	mVertices = 0;
	mIndices = 0;
}

void GLMesh::Bind() const
{
	assert(mVertices != 0);
	assert(mIndices != 0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);
}

void GLMesh::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

size_t GLMesh::GetNumIndices() const
{
	assert(mVertices != 0);
	assert(mIndices != 0);
	return(mNumIndices);
}
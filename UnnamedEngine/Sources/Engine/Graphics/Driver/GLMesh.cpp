#include "GLMesh.h"
#include <memory>

#include "Engine/Base/Resource/MeshResource.h"

GLMesh::GLMesh(const std::weak_ptr<MeshResource>& resource)
{
	auto res = resource.lock();

	//Bind VBO
	glGenBuffers(1, &mVertices);
	glBindBuffer(GL_ARRAY_BUFFER, mVertices);
	const auto& vertices = res->GetVertices();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


	// Bind EBO
	glGenBuffers(1, &mIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);
	const auto& indices = res->GetIndices();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);
	mSize = indices.size();
}

void GLMesh::Free()
{
	glDeleteBuffers(1, &mIndices);
	glDeleteBuffers(1, &mVertices);
}

size_t GLMesh::GetSize() const
{
	return(mSize);
}
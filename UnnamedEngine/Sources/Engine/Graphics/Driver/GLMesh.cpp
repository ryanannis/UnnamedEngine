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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);


	// Bind EBO
	glGenBuffers(1, &mIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndices);
	const auto& indices = res->GetIndices();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);
	mSize = indices.size();
}

GLMesh::~GLMesh()
{
	glDeleteShader(mIndices);
	glDeleteShader(mVertices);
}

size_t GLMesh::GetSize() const
{
	return(mSize);
}
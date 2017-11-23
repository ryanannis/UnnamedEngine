#include "GLMesh.h"
#include <memory>
#include <glad/glad.h>

#include "Engine/Base/Resource/MeshResource.h"

GLMesh::GLMesh(const std::weak_ptr<MeshResource>& resource)
{
	auto res = resource.lock();

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	const auto& vertices = res->GetVertices();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);


}





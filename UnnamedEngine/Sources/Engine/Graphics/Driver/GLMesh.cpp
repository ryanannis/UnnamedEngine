#include "GLMesh.h"
#include <memory>

#include "Engine/Base/Resource/ModelResource.h"

GLMesh::GLMesh(const std::weak_ptr<ModelResource>& resource)
{
	auto res = resource.lock();
	const auto& meshes = res->GetMeshes();
	for(auto mesh : meshes)
	{
		GLuint verticeBuffer;
		GLuint indicesBuffer;

		//Bind VBO
		glGenBuffers(1, &verticeBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, verticeBuffer);
		const auto& vertices = mesh.mVertices;
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), vertices.data(), GL_STATIC_DRAW);


		// Bind EBO
		glGenBuffers(1, &indicesBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
		const auto& indices = mesh.mIndices;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

		GLSubmesh s;
		s.indicesBuffer = indicesBuffer;
		s.verticesbuffer = verticeBuffer;
		s.numIndices = indices.size();

		mSubmeshes.push_back(s);
	}
}

void GLMesh::Free()
{
	for(const auto& submesh : mSubmeshes)
	{
		glDeleteBuffers(1, &submesh.indicesBuffer);
		glDeleteBuffers(1, &submesh.verticesbuffer);
	}
}

const std::vector<GLSubmesh>& GLMesh::GetSubmeshes() const
{
	return(mSubmeshes);
}
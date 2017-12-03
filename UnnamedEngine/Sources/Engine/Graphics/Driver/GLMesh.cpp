#include "GLMesh.h"
#include <memory>

#include "Engine/Base/Resource/ModelResource.h"
#include "Engine/Graphics/Driver/GLDriver.h"

GLMesh::GLMesh(const std::shared_ptr<ModelResource>& resource, GLDriver* driver)
{
	auto res = resource;
	const auto& meshes = res->GetMeshes();
	for(auto mesh : meshes)
	{
		GLuint verticeBuffer;
		GLuint indicesBuffer;

		//Bind VBO of interleaved vertices
		glGenBuffers(1, &verticeBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, verticeBuffer);
		const auto& vertices = mesh.mVectors;
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
		
		// assigne texes
		// todo:  why would a mesh have multiple texs?
		if(mesh.mDiffuseTextures.size() > 0)
		{
			s.diffuse = driver->LoadTexture(mesh.mDiffuseTextures[0]);
		}
		if(mesh.mSpecularTextures.size() > 0)
		{
			s.specular = driver->LoadTexture(mesh.mSpecularTextures[0]);
		}

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
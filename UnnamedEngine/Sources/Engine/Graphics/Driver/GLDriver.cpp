#include "GLDriver.h"

#include <GLFW/glfw3.h>

#include "Engine/Base/Resource/URI.h"
#include "Engine/Base/Resource/ResourceManager.h"

#include "Engine/Base/Resource/ModelResource.h"
#include "Engine/Base/Resource/MaterialResource.h"
#include "Engine/Base/Resource/ShaderResource.h"

#include "Engine/Graphics/Driver/GLMesh.h"
#include "Engine/Graphics/Driver/GLProgram.h"
#include "Engine/Graphics/Driver/GLShader.h"
#include "Engine/Graphics/Driver/GLAttributes.h"
#include "Engine/Graphics/Driver/GLTexture.h"

GLDriver::GLDriver(Ptr<ResourceManager> manager)
	: mResourceManager(manager)
{
}

void GLDriver::EnableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
}

void GLDriver::ClearFramebuffer(uint8_t r, uint8_t b, uint8_t g)
{
	glClearColor(r, b, g, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLDriver::ClearResources()
{
	for(auto& mesh : mMeshes)
	{
		mesh.second.Free();
	}
	for(auto& texture : mTextures)
	{
		texture.second.Free();
	}
	for(auto& program: mPrograms)
	{
		program.Free();
	}
	for(auto& attribute : mAttributes)
	{
		attribute.Free();
	}

	mMeshes.clear();
	mPrograms.clear();
	mAttributes.clear();
}

Ptr<GLMesh> GLDriver::CreateMesh(const ResourceType<ModelResource>& modelResource)
{
	// Check if already loaded
	const auto& key = modelResource.GetURI();
	const auto& hash = key.GetHash();
	auto mesh = mMeshes.find(hash);
	if(mesh != mMeshes.end())
	{
		return(&mesh->second);
	}

	auto loadedMesh = mResourceManager->LoadResource(modelResource);
	auto meshPointer = mMeshes.emplace(hash, GLMesh(loadedMesh, this));
	return(&(meshPointer.first->second));
}

Ptr<GLTexture> GLDriver::LoadTexture(const ResourceType<MaterialResource>& textureResource)
{
	// Check if already loaded
	auto key = textureResource.GetURI();
	auto hash = key.GetHash();
	auto texture = mTextures.find(hash);
	if(texture != mTextures.end())
	{
		return(&texture->second);
	}

	auto loadedTexture = mResourceManager->LoadResource(textureResource);
	auto texturePointer = mTextures.emplace(hash, GLTexture(loadedTexture));
	return(&(texturePointer.first->second));
}

void GLDriver::DrawMesh(Ptr<GLAttributes> vao, Ptr<GLMesh> mesh)
{
	for(const GLSubmesh& submesh : mesh->GetSubmeshes())
	{

		//auto submesh = mesh->GetSubmeshes()[3];
		if(submesh.diffuse)
		{
			submesh.diffuse->Bind(0);
		}
		if(submesh.specular)
		{
			submesh.specular->Bind(1);
		}

		vao->Bind();
		glBindBuffer(GL_ARRAY_BUFFER, submesh.verticesbuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, submesh.indicesBuffer);

		vao->AddAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), 0); // vertexes
		vao->AddAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), 3 * sizeof(float)); // normals
		vao->AddAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), 6 * sizeof(float)); // uvs
		glDrawElements(GL_TRIANGLES, submesh.numIndices, GL_UNSIGNED_INT, 0);
		vao->Unbind();
	}
}

Ptr<GLProgram> GLDriver::CreateProgram(
	const Ptr<GLAttributes>& vao,
	const std::shared_ptr<ShaderResource>& vertShader,
	const std::shared_ptr<ShaderResource>& fragShader
)
{
	vao->Bind();

	GLProgram program;
	GLShader vert(vertShader);
	GLShader frag(fragShader);
	program.RegisterShader(vert);
	program.RegisterShader(frag);

	program.Link();
	vert.Free();
	frag.Free();

	mPrograms.emplace_back(std::move(program));

	return(&mPrograms.back());
}

Ptr<GLAttributes> GLDriver::CreateAttributes()
{
	GLAttributes attributes;
	mAttributes.push_back(std::move(attributes));
	return(&mAttributes.back());
}

void GLDriver::SwapBuffers(GLFWwindow* window)
{
	glfwSwapBuffers(window);
}

#include "GLDriver.h"

#include <GLFW/glfw3.h>

#include "Engine/Base/Resource/URI.h"
#include "Engine/Base/Resource/ResourceManager.h"

#include "Engine/Base/Resource/ModelResource.h"
#include "Engine/Base/Resource/MaterialResource.h"
#include "Engine/Base/Resource/ShaderResource.h"

#include "Engine/Graphics/Driver/GLModel.h"
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
	for(auto& mesh : mModels)
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

	mModels.clear();
	mPrograms.clear();
	mAttributes.clear();
}

Ptr<GLModel> GLDriver::CreateModel(const ResourceType<ModelResource>& modelResource)
{
	// Check if already loaded
	const auto& key = modelResource.GetURI();
	const auto& hash = key.GetHash();
	auto mesh = mModels.find(hash);
	if(mesh != mModels.end())
	{
		return(&mesh->second);
	}

	auto loadedMesh = mResourceManager->LoadResource(modelResource);
	auto meshPointer = mModels.emplace(hash, GLModel(loadedMesh, this));
	return(&(meshPointer.first->second));
}

Ptr<GLMesh> GLDriver::CreateMesh(const std::vector<float>& vertices, const std::vector<GLuint>& indices)
{
	mMeshes.push_back(GLMesh(vertices, indices));
	return(&mMeshes.back());
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

void GLDriver::DrawMesh(Ptr<GLAttributes> vao, Ptr<GLModel> mesh)
{
	for(const GLSubModel& submesh : mesh->GetSubmeshes())
	{
		vao->Bind();
		if(submesh.diffuse)
		{
			submesh.diffuse->Bind(10);
		}
		if(submesh.specular)
		{
			submesh.specular->Bind(11);
		}

		submesh.mesh.Bind();

		vao->AddAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), 0); // vertexes
		vao->AddAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), 3 * sizeof(float)); // normals
		vao->AddAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), 6 * sizeof(float)); // uvs
		glDrawElements(GL_TRIANGLES, submesh.mesh.GetNumIndices(), GL_UNSIGNED_INT, 0);
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

Ptr<GLRenderTarget> GLDriver::CreateRenderTarget()
{
	GLRenderTarget target;
	mRenderTargets.push_back(std::move(target));
	return(&mRenderTargets.back());
}

void GLDriver::SwapBuffers(GLFWwindow* window)
{
	glfwSwapBuffers(window);
}

#include "Renderer.h"
#include "Engine/Base/Common/Common.h"

#include "Engine/Graphics/Driver/GLMesh.h"
#include "Engine/Graphics/Driver/GLProgram.h"
#include "Engine/Graphics/Driver/GLShader.h"
#include "Engine/Graphics/Driver/GLAttributes.h"

#include "Engine/Base/Resource/MeshResource.h"

#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Client/Client.h"
#include "Engine/Base/Resource/ResourceManager.h"


#include <glad/glad.h> // haven't abstracted the enums yet

static size_t sGraphicsHandleCt;

Renderer::Renderer(Ptr<Context> c) :
	mContext{c}
{
	mDriver = std::make_unique<GLDriver>();
}

// This comes in after the resource manager is done
void Renderer::Initialize()
{
	ResourceType<ShaderResource> basicVertType("Engine/Basic.vert");
	ResourceType<ShaderResource> basicFragType("Engine/Basic.frag");
	mBasicVert = mContext->GetResourceManager()->LoadResource(basicVertType);
	mBasicFrag = mContext->GetResourceManager()->LoadResource(basicFragType);
}

const CameraData& Renderer::GetCameraData() const
{
	return(mCameraData);
}

void Renderer::SetCameraData(const CameraData& data)
{
	mCameraData = data;
}

Matrix4 Renderer::GetCameraVPMatrix()
{
	const auto viewMat = glm::mat4_cast(mCameraData.rotation);
	const auto perspectiveMat = glm::perspective(mCameraData.fov, mCameraData.aspectRatio, 0.01f, 1000.0f);
	return(perspectiveMat * glm::inverse(viewMat));
}


void Renderer::Render()
{
	mDriver->ClearFramebuffer(0, 0, 0);

	RenderMeshes();

	mDriver->SwapBuffers(mContext->GetClient()->GetWindow());
	mDriver->ClearResources();
}

void Renderer::RenderMeshes()
{
	// Generate VAO
	auto vao = mDriver->CreateAttributes();
	vao->Bind();

	// Generate Programs
	auto basicProgram = mDriver->CreateProgram(vao, mBasicVert, mBasicFrag);
	basicProgram->SetUniformMatrix4("MVP", GetCameraVPMatrix());
	basicProgram->Bind();

	for(const GraphicsData& g : mGraphicsData)
	{
		vao->Bind();
		auto meshWkRes = mContext->GetResourceManager()->LoadResource(g.mesh);
		auto mesh = mDriver->CreateMesh(meshWkRes);
		vao->AddAttribute(0, 3, GL_FLOAT, 3 * sizeof(float));
		mDriver->DrawElements(mesh->GetSize());
	}

}


// Generates an object which represents the data needed
// to render an entity
GraphicsHandle Renderer::GenerateGraphicsData()
{
	// Register object
	GraphicsHandle g(++sGraphicsHandleCt);

	GraphicsData data;
	mGraphicsData.push_back(data);

	return(g);
}

GraphicsData& Renderer::GetGraphicsData(const GraphicsHandle& handle)
{
	assert(handle.id != 0); // invalid handle
	return(mGraphicsData[handle.id - 1]);
}


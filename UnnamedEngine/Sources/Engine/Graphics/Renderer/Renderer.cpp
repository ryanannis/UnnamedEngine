#include "Renderer.h"
#include "Engine/Base/Common/Common.h"

#include "Engine/Graphics/Driver/GLMesh.h"
#include "Engine/Graphics/Driver/GLProgram.h"
#include "Engine/Graphics/Driver/GLShader.h"
#include "Engine/Graphics/Driver/GLAttributes.h"

#include "Engine/Base/Resource/ModelResource.h"

#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Client/Client.h"
#include "Engine/Base/Resource/ResourceManager.h"

#include <math.h>


#include <glad/glad.h> // haven't abstracted the enums yet

static size_t sGraphicsHandleCt;

Renderer::Renderer(Ptr<Context> c) :
	mContext{c}
{
}

// This comes in after the resource manager is done
void Renderer::Initialize()
{
	ResourceType<ShaderResource> basicVertType("Engine/Basic.vert");
	ResourceType<ShaderResource> basicFragType("Engine/Basic.frag");
	mBasicVert = mContext->GetResourceManager()->LoadResource(basicVertType);
	mBasicFrag = mContext->GetResourceManager()->LoadResource(basicFragType);
	mDriver = std::make_unique<GLDriver>(mContext->GetResourceManager());

	// Generate VAO
	mVao = mDriver->CreateAttributes();
	mVao->Bind();

	// Generate Programs
	mBasicProgram = mDriver->CreateProgram(mVao, mBasicVert, mBasicFrag);
	mBasicProgram->Bind();
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
	// Vector look
	// Build camera view matrix

	const auto& camRot = mCameraData.rotation;
	const Vector3f lookDirection(cos(camRot.y) * cos(camRot.x), sin(camRot.y), cos(camRot.y) * sin(camRot.x));
	const Vector3f up(0.f, 1.f, 0.f);
	const auto viewMat = glm::lookAt(
		mCameraData.translation,
		mCameraData.translation + glm::normalize(lookDirection),
		up
	);

	const auto perspectiveMat = glm::perspective(mCameraData.fov, mCameraData.aspectRatio, 0.01f, 1000.0f);
	return(perspectiveMat * viewMat);
}


void Renderer::Render()
{
	mDriver->ClearFramebuffer(0, 0, 0);

	RenderMeshes();

	mDriver->SwapBuffers(mContext->GetClient()->GetWindow());
}

void Renderer::RenderMeshes()
{
	mVao->Bind();
	mBasicProgram->Bind();
	for(const GraphicsData& g : mGraphicsData)
	{
		auto mesh = mDriver->CreateMesh(g.mesh);

		mVao->AddAttribute(0, 3, GL_FLOAT, 8 * sizeof(float), 0); // vertexes
		mVao->AddAttribute(1, 3, GL_FLOAT, 8 * sizeof(float), 3 * sizeof(float)); // normals
		mVao->AddAttribute(2, 2, GL_FLOAT, 8 * sizeof(float), 6 * sizeof(float)); // uvs

		mBasicProgram->SetUniformMatrix4("MVP", GetCameraVPMatrix() * glm::translate(g.translation));
		mBasicProgram->SetUniformInt("DiffuseSampler", 0);
		mBasicProgram->SetUniformInt("SpecularSampler", 1);
		mDriver->DrawMesh(mesh);
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


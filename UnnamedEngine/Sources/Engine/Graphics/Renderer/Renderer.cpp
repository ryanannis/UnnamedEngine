#include "Renderer.h"
#include "Engine/Base/Common/Common.h"

#include "Engine/Graphics/Driver/GLModel.h"
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
	mDriver = std::make_unique<GLDriver>(mContext->GetResourceManager());

	// For drawing fullscreen quad 
	const static auto fsQuadVerts = std::vector<float>{
		-1.0, -1.0, -1.0, //top left corner
		0.f, 0.f,		  //texture top left
		-1.0, 1.0, -1.0,  //bottom left corner
		0.f, 1.f,		  //texture bottom left
		1.0, -1.0, -1.0,  //top right corner
		1.f, 0.f,		  //
		1.0, 1.0, -1.0,   //bottom right corner
		1.f, 1.f        // 
	};

	const static auto fsQuadIndices = std::vector<GLuint>{
		2, 1, 0,
		2, 3, 1
	};
}

const CameraData& Renderer::GetCameraData() const
{
	return(mCameraData);
}

void Renderer::SetCameraData(const CameraData& data)
{
	mCameraData = data;
}

void Renderer::DrawFullscreenQuad()
{
	mBasicProgram->Bind();

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

	const auto perspectiveMat = glm::perspective(
		mCameraData.fov,
		mCameraData.aspectRatio,
		0.1f,
		10000.0f
	);

	return(perspectiveMat * viewMat);
}


void Renderer::Render()
{
}

void Renderer::RenderMeshes()
{
}

void Renderer::RenderGBuffer()
{
}

void Renderer::RenderLights()
{
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


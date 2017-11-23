#include "Renderer.h"
#include "Engine/Base/Common/Common.h"
#include "Engine/Graphics/Driver/GLDriver.h"
#include "Engine/Graphics/Driver/GLMesh.h"
#include "Engine/Graphics/Driver/GLProgram.h"
#include "Engine/Graphics/Driver/GLShader.h"

static size_t sGraphicsHandleCt;

Renderer::Renderer()
{
	mDriver = std::make_unique<GLDriver>();
}

const CameraData& Renderer::GetCameraData() const
{
	return(mCameraData);
}

void Renderer::SetCameraData(const CameraData& data)
{
	mCameraData = data;
}

void Renderer::Render()
{
	for(const GraphicsData& g : mGraphicsData)
	{
		Matrix4 viewMat = glm::mat4_cast(mCameraData.rotation);

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

GraphicsData& Renderer::GetGraphicsData(GraphicsHandle& handle)
{
	assert(handle.id != 0); // invalid handle
	return(mGraphicsData[handle.id - 1]);
}
#include "Renderer.h"
#include "Engine/Base/Common/Common.h"

#include "Engine/Graphics/Driver/GLMesh.h"
#include "Engine/Graphics/Driver/GLProgram.h"
#include "Engine/Graphics/Driver/GLShader.h"
#include "Engine/Graphics/Driver/GLAttributes.h"

#include "Engine/Base/Resource/MeshResource.h"

#include "Engine/Base/Client/Context.h"
#include "Engine/Base/Resource/ResourceManager.h"

#include <glad/glad.h> // haven't abstracted the enums yet

static size_t sGraphicsHandleCt;

Renderer::Renderer(Ptr<Context> c) :
	mContext{c}
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
	// this is temp as I don't have structure for render passes setup atm
	ResourceType<ShaderResource> basicVertType("Engine/Basic.vert");
	ResourceType<ShaderResource> basicFragType("Engine/Basic.frag");
	auto basicVert = mContext->GetResourceManager()->LoadResource(basicVertType);
	auto basicFrag = mContext->GetResourceManager()->LoadResource(basicFragType);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	for(const GraphicsData& g : mGraphicsData)
	{
		Matrix4 viewMat = glm::mat4_cast(mCameraData.rotation);
		glm::translate(viewMat, mCameraData.translation);
		auto meshWkRes = mContext->GetResourceManager()->LoadResource(g.mesh);

		// GL stuff for drawing a mesh - should be moved in the future
		auto basicProgram = mDriver->CreateProgram(basicVert, basicFrag);
		basicProgram->SetUniformMatrix4("MVP", viewMat);
		auto basicProgramAttributes = mDriver->CreateAttributes();
		basicProgramAttributes->Bind();
		auto mesh = mDriver->CreateMesh(meshWkRes);
		basicProgramAttributes->AddAttribute(0, 3, GL_FLOAT, 3 * sizeof(float));
		glDrawElements(GL_TRIANGLES, mesh->GetSize(), GL_UNSIGNED_INT, 0);
	}
	mDriver->ClearResources();
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
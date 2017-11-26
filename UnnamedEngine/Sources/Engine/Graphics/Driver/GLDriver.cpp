#include "GLDriver.h"

#include "Engine/Graphics/Driver/GLMesh.h"
#include "Engine/Graphics/Driver/GLProgram.h"
#include "Engine/Graphics/Driver/GLShader.h"
#include "Engine/Graphics/Driver/GLAttributes.h"

void GLDriver::ClearResources()
{
	mMeshes.clear();
	mPrograms.clear();
	mAttributes.clear();
}

Ptr<GLMesh> GLDriver::CreateMesh(const std::weak_ptr<MeshResource>& meshResource)
{
	GLMesh mesh(meshResource);
	mMeshes.push_back(std::move(meshResource));
	return(&mMeshes.back());
}

Ptr<GLProgram> GLDriver::CreateProgram(
	const std::weak_ptr<ShaderResource>& vertShader,
	const std::weak_ptr<ShaderResource>& fragShader
)
{
	// need to use emplace_back here since program is noncopyable
	GLProgram program;
	program.RegisterShader(GLShader(vertShader));
	program.RegisterShader(GLShader(fragShader));
	program.Link();

	mPrograms.emplace_back(std::move(program));

	return(&mPrograms.back());
}

Ptr<GLAttributes> GLDriver::CreateAttributes()
{
	GLAttributes attributes;
	mAttributes.push_back(std::move(attributes));
	return(&mAttributes.back());
}

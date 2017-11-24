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
	mMeshes.emplace_back(meshResource);
	return(&mMeshes.back());
}

Ptr<GLProgram> GLDriver::CreateProgram(
	const std::weak_ptr<ShaderResource>& vertShader,
	const std::weak_ptr<ShaderResource>& fragShader
)
{
	// These can can be deleted after putting them into the program

	GLProgram program;
	program.RegisterShader(GLShader(vertShader));
	program.RegisterShader(GLShader(fragShader));
	program.Link();
	mPrograms.emplace_back(program);

	return(&mPrograms.back());
}

Ptr<GLAttributes> GLDriver::CreateAttributes()
{
	mAttributes.emplace_back();
	return(&mAttributes.back());
}

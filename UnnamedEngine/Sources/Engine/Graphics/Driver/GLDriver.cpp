#include "GLDriver.h"

#include "Engine/Graphics/Driver/GLMesh.h"
#include "Engine/Graphics/Driver/GLProgram.h"
#include "Engine/Graphics/Driver/GLShader.h"
#include "Engine/Graphics/Driver/GLAttributes.h"


GLMesh GLDriver::CreateMesh(const std::weak_ptr<MeshResource>& meshResource)
{
	return(GLMesh(meshResource));
}

GLProgram GLDriver::CreateProgram(
	const std::weak_ptr<ShaderResource>& vertShader,
	const std::weak_ptr<ShaderResource>& fragShader
)
{
	// These can can be deleted after putting them into the program
	GLShader vert = GLShader(vertShader);
	GLShader frag = GLShader(fragShader);

	GLProgram program;
	program.RegisterShader(vert);
	program.RegisterShader(frag);
	program.Link();

	return(program);
}

GLAttributes GLDriver::CreateAttributes()
{
	return(GLAttributes());
}

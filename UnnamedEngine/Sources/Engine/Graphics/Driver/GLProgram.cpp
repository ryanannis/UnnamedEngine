#include "GLProgram.h"

#include <assert.h>
#include <memory>
#include <glad/glad.h>
#include "Engine/Graphics/Driver/GLCommon.h"


GLProgram::GLProgram()
{
	mProgramHandle = glCreateProgram();
}

GLuint GLProgram::GetProgramHandle() const
{
	return(mProgramHandle);
}

void GLProgram::RegisterShader(std::weak_ptr<ShaderResource> resource)
{
	mShaders.push_back(GLShader(resource));
}

void GLProgram::Link()
{
	for(auto shader : mShaders)
	{
		glAttachShader(mProgramHandle, shader.GetShaderHandle());
		glAttachShader(mProgramHandle, shader.GetShaderHandle());
	}

	glLinkProgram(mProgramHandle);

	int success;
	char infoLog[512];
	glGetProgramiv(mProgramHandle, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(mProgramHandle, 512, NULL, infoLog);
		assert(0);
	}
}

void GLProgram::Bind()
{
	glUseProgram(mProgramHandle);
}

void GLProgram::Unbind()
{
	glUseProgram(0);
}
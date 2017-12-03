#include "GLShader.h"

#include <memory>
#include <glad/glad.h>

#include "Engine/Base/Resource/ShaderResource.h"

#include "Engine/Graphics/Driver/GLCommon.h"

GLShader::GLShader(const std::shared_ptr<ShaderResource>& resource)
{
	std::shared_ptr<ShaderResource> res = resource;
	auto shaderType = res->GetShaderType();

	if(shaderType == ShaderType::VERTEX){
		mShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	}
	else if(shaderType == ShaderType::FRAGMENT) {
		mShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else if (shaderType == ShaderType::GEOMETRY) {
		mShaderHandle = glCreateShader(GL_GEOMETRY_SHADER);
	}
	else if (shaderType == ShaderType::COMPUTE) {
		mShaderHandle = glCreateShader(GL_COMPUTE_SHADER);
	}

	const char *shaderTextStr = res->GetShaderText().data();

	glShaderSource(mShaderHandle, 1, &shaderTextStr, NULL);
	glCompileShader(mShaderHandle);

	int success;
	char infoLog[MAX_LOG_SIZE];
	glGetShaderiv(mShaderHandle, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(mShaderHandle, MAX_LOG_SIZE, NULL, infoLog);
		fprintf(stderr, "Compilation error in shader: %s\n", infoLog);
		assert(false);
	}
}

void GLShader::Free()
{
	if(mShaderHandle != 0)
	{
		glDeleteShader(mShaderHandle);
	}
}

GLuint GLShader::GetShaderHandle() const
{
	return(mShaderHandle);
}
#include "GLProgram.h"

#include <assert.h>
#include <memory>
#include <glad/glad.h>
#include "Engine/Graphics/Driver/GLCommon.h"


GLProgram::GLProgram()
{
	mProgramHandle = glCreateProgram();
}

void GLProgram::Free()
{
	glDeleteProgram(mProgramHandle);
}

GLuint GLProgram::GetProgramHandle() const
{
	return(mProgramHandle);
}

void GLProgram::RegisterShader(const GLShader& shader)
{
	glAttachShader(mProgramHandle, shader.GetShaderHandle());
}

void GLProgram::Link()
{
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

GLuint GLProgram::GetUniformLocation(const char* c)
{
	return(glGetUniformLocation(GetProgramHandle(), c));
}

void GLProgram::SetUniformVertex4f(const char* name, Vector4f val)
{
	Bind();
	GLuint uniform = GetUniformLocation(name);
	glUniform4f(uniform, val.x, val.y, val.z, val.w);
}

void GLProgram::SetUniformInt(const char* name, int i)
{
	GLuint uniform = GetUniformLocation(name);
	glUniform1i(uniform, i);
}

void GLProgram::SetUniformFloat(const char* name, float f)
{
	GLuint uniform = GetUniformLocation(name);
	glUniform1f(uniform, f);
}

void GLProgram::SetUniformMatrix4(const char* name, const Matrix4& mat)
{
	GLuint uniform = GetUniformLocation(name);
	glUniformMatrix4fv(uniform, 1, GL_FALSE, &mat[0][0]);
}
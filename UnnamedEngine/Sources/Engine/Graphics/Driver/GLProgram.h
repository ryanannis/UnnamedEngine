#pragma once
#include <glad/glad.h>
#include <memory>
#include <vector>

#include "Engine/Graphics/Driver/GLShader.h"

class GLProgram
{
public:
	GLProgram();
	void RegisterShader(const GLShader& shader);
	void Link();
	void Bind();
	void Unbind();
	GLuint GetProgramHandle() const;
	
	void SetUniformVertex4f(const char* name, Vector4f val);
	void SetUniformInt(const char* name, int i);
	void SetUniformFloat(const char* name, float f);

private:
	GLuint GetUniformLocation(const char* c);

	std::vector<GLShader> mShaders;
	GLuint mProgramHandle;
};
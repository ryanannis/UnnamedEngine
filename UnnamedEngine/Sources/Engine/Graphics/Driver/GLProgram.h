#pragma once
#include "Engine/Base/Common/Common.h"

#include <glad/glad.h>

#include <memory>
#include <vector>

#include "Engine/Graphics/Driver/GLShader.h"

class GLProgram : public NonCopyable
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
	void SetUniformMatrix4(const char* name, const Matrix4& mat);

private:
	GLuint GetUniformLocation(const char* c);

	std::vector<GLShader> mShaders;
	GLuint mProgramHandle;
};
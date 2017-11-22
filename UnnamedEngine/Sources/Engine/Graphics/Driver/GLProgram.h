#pragma once
#include <glad/glad.h>
#include <memory>
#include <vector>

#include "Engine/Graphics/Driver/GLShader.h"

class GLProgram
{
public:
	void RegisterShader(std::weak_ptr<ShaderResource> resource);
	void Link();
	void Bind();
	void Unbind();
	GLuint GetProgramHandle() const;

private:
	std::vector<GLShader> mShaders;
	GLuint mProgramHandle;
};
#pragma once
#include "Engine/Base/Common/Common.h"
#include <glad/glad.h>
#include <memory>

class ShaderResource;

class GLShader
{
public:
	GLShader(const std::weak_ptr<ShaderResource>& resource);
	void Free();
	GLuint GetShaderHandle() const;

private:
	GLuint mShaderHandle;
};
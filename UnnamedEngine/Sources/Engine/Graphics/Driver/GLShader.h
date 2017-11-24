#pragma once
#include "Engine/Base/Common/Common.h"
#include <glad/glad.h>
#include <memory>

class ShaderResource;

class GLShader : public NonCopyable
{
public:
	GLShader(const std::weak_ptr<ShaderResource>& resource);
	~GLShader();
	GLuint GetShaderHandle() const;

private:
	GLuint mShaderHandle;
};
#pragma once
#include <glad/glad.h>
#include <memory>

class ShaderResource;

class GLShader
{
public:
	GLShader(std::weak_ptr<ShaderResource> resource);
	GLuint GetShaderHandle() const;

private:
	GLuint mShaderHandle;
	std::weak_ptr<ShaderResource> mResource;

};
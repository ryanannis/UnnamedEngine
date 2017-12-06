#pragma once
#include <glad/glad.h>
#include <vector>
#include "Engine/Graphics/Driver/GLTexture.h"

class GLRenderTarget
{
public:
	GLRenderTarget();

	GLuint GetHandle();
	void Bind();
	void Unbind();

	void Free();

private:
	GLuint mTexture;
	GLuint mFbo;
};


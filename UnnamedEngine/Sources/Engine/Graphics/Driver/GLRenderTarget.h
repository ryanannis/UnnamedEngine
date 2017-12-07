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

	// render buffers
	GLuint mDiffuse;
	GLuint mPosition;
	GLuint mNormals;
	GLuint mDepth;
	
	// textures
	GLuint mDiffuseTex;
	GLuint mPositionTex;
	GLuint mNormalTex;

	GLuint mTexture;
	GLuint mFbo;
};


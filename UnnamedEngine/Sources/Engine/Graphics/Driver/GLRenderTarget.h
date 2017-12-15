#pragma once
#include "Engine/Base/Common/Common.h"
#include <glad/glad.h>
#include <vector>
#include "Engine/Graphics/Driver/GLTexture.h"

class GLTexture;

class GLRenderTarget
{
public:
	GLRenderTarget(size_t width, size_t height, size_t numTargets, bool requireDepths);

	GLuint GetHandle();
	void Bind(size_t textureSlot);
	void Unbind();
	void SetTarget(size_t index, Ptr<GLTexture> texture);

	void Free();

private:
	std::vector<GLuint> mRenderBuffers;
	std::vector<GLuint> mTextures;
	std::vector<GLuint> mColorAttachments;
	GLuint mDepthRenderBuffer;
	
	size_t mWidth;
	size_t mHeight;
	bool mHasDepth;
	GLuint mFbo;
};


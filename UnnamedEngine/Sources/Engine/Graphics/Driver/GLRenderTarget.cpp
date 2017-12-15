#include "GLRenderTarget.h"
#include "GLTexture.h"

GLRenderTarget::GLRenderTarget(size_t width, size_t height, size_t numTargets, bool requireDepths)
{
	// OpenGL allows 16 renderbuffer color attachments.
	// This is probably more than is ever needed.
	assert(numTargets < 16);
	glGenFramebuffers(1, &mFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

	mWidth = width;
	mHeight = height;
	mHasDepth = requireDepths;

	mRenderBuffers = std::vector<GLuint>(numTargets);
	mTextures = std::vector<GLuint>(numTargets);

	// bleh... but OGL wants it like this
	mColorAttachments = std::vector<GLuint>(numTargets);
	for(size_t i = 0; i < numTargets; i++)
	{
		mColorAttachments[i] = GL_COLOR_ATTACHMENT0 + i;
	}

	if(requireDepths)
	{
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRenderBuffer);
	}
}

GLuint GLRenderTarget::GetHandle()
{
	return(mFbo);
}

void GLRenderTarget::Bind(size_t textureSlot)
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, mWidth, mHeight);

	glClear(
		GL_COLOR_BUFFER_BIT |
		(GL_DEPTH_BUFFER_BIT ? GL_DEPTH_BITS : 0)
	);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glEnable(GL_TEXTURE_2D);
	
	glDrawBuffers(mColorAttachments.size(), mColorAttachments.data());
}

void GLRenderTarget::SetTarget(size_t index, Ptr<GLTexture> texture)
{
	assert(index < mRenderBuffers.size());
	assert(texture->GetWidth() == mWidth);
	assert(texture->GetHeight() == mHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

	GLuint renderBufferHandle;
	glGenRenderbuffers(1, &renderBufferHandle);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferHandle);
	glRenderbufferStorage(GL_RENDERBUFFER, texture->GetTextureType(), mWidth, mHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, renderBufferHandle);
}

void GLRenderTarget::Free()
{
	glDeleteFramebuffers(1, &mFbo);
}

void GLRenderTarget::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glPopAttrib();
}
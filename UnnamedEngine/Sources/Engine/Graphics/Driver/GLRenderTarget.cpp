#include "GLRenderTarget.h"

GLRenderTarget::GLRenderTarget()
{
	glGenFramebuffers(1, &mFbo);

	// temp: deferred rendering stuff
	// todo:  abstract this crap
	static const size_t width = 1280;
	static const size_t height = 720;

	glGenRenderbuffers(1, &mDiffuse);
	glGenRenderbuffers(1, &mPosition);
	glGenRenderbuffers(1, &mNormals);
	glGenRenderbuffers(1, &mDepth);

	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

	glBindRenderbuffer(GL_RENDERBUFFER, mDiffuse);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, mDiffuse);

	glBindRenderbuffer(GL_RENDERBUFFER, mPosition);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, mPosition);

	glBindRenderbuffer(GL_RENDERBUFFER, mNormals);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA16F, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_RENDERBUFFER, mNormals);

	glBindRenderbuffer(GL_RENDERBUFFER, mDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepth);

	glGenTextures(1, &mDiffuseTex);
	glGenTextures(1, &mPositionTex);
	glGenTextures(1, &mNormalTex);

	glBindTexture(GL_TEXTURE_2D, mDiffuseTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mDiffuseTex, 0);

	glBindTexture(GL_TEXTURE_2D, mPositionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mPositionTex, 0);

	glBindTexture(GL_TEXTURE_2D, mNormalTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mNormalTex, 0);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(status == GL_FRAMEBUFFER_COMPLETE_EXT);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GLRenderTarget::GetHandle()
{
	return(mFbo);
}

void GLRenderTarget::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, 1280, 720); //todo

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	
	GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 }; // todo
	glDrawBuffers(3, buffers);
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
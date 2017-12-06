#include "GLRenderTarget.h"

GLRenderTarget::GLRenderTarget()
{
	glGenFramebuffers(1, &mFbo);
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

	glActiveTexture(GL_TEXTURE0_ARB);
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
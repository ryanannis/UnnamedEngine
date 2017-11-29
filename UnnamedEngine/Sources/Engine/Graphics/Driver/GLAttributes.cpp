#include "GLAttributes.h"

GLAttributes::GLAttributes()
{
	glGenVertexArrays(1, &mVAO);
}

void GLAttributes::AddAttribute(
	GLuint index,
	GLint size,
	GLenum type,
	GLsizei stride
)
{
	Bind();
	glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void*)0);
}

GLuint GLAttributes::GetHandle()
{
	return(mVAO);
}

void GLAttributes::Bind()
{
	glBindVertexArray(mVAO);
}

void GLAttributes::Free()
{
	glDeleteVertexArrays(1, &mVAO);
}

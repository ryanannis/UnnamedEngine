#include "GLAttributes.h"

GLAttributes::GLAttributes()
{
	glGenVertexArrays(1, &mVAO);
}

void GLAttributes::AddAttribute(
	GLuint index,
	GLint size,
	GLenum type,
	GLsizei stride,
	GLsizei start
)
{
	Bind();
	glVertexAttribPointer(index, size, type, GL_FALSE, stride, (void*)start);
	glEnableVertexAttribArray(index);
	mAttributeIndices.push_back(index);
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
	for(auto indice : mAttributeIndices)
	{
		glEnableVertexAttribArray(indice);
	}
}

void GLAttributes::Unbind()
{
	glBindVertexArray(mVAO);
}
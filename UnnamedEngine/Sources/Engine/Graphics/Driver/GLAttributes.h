#pragma once
#include <glad/glad.h>
#include <vector>

class GLAttributes
{
public:
	GLAttributes();

	void AddAttribute(
		GLuint index,
		GLint size,
		GLenum type,
		GLsizei stride,
		GLsizei start
	);

	GLuint GetHandle();
	void Bind();

	void Free();

private:
	std::vector<GLuint> mAttributeIndices;
	GLuint mVAO;
};


#include "GLTexture.h"

#include <memory>
#include <glad/glad.h>

#include "Engine/Base/Resource/MaterialResource.h"

#include "Engine/Graphics/Driver/GLCommon.h"

GLTexture::GLTexture(const std::shared_ptr<MaterialResource>& resource)
{
	const auto& texture = resource->GetTexture();
	glGenTextures(1, &mTextureHandle);
	glBindTexture(GL_TEXTURE_2D, mTextureHandle);

	// todo:  configurable
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void GLTexture::Free()
{
	glDeleteTextures(1, &mTextureHandle);
}

GLuint GLTexture::GetTextureHandle() const
{
	return(mTextureHandle);
}

void GLTexture::Bind(size_t unit)
{
	auto glUnit = GL_TEXTURE0 + unit;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTextureHandle);
}
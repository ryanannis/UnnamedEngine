#include "GLTexture.h"

#include <memory>
#include <glad/glad.h>

#include "Engine/Base/Resource/MaterialResource.h"

#include "Engine/Graphics/Driver/GLCommon.h"


GLTexture::GLTexture(GLuint resourcePtr, size_t width, size_t height, GLuint textureType) :
	mTextureHandle(resourcePtr),
	mWidth(width),
	mHeight(height),
	mTextureType(textureType)
{
}

GLTexture::GLTexture(const std::shared_ptr<MaterialResource>& resource)
{
	if(!resource->IsReady())
	{
		// If the texture is for some reason broken, then just let it get bound to 0 (black)
		return;
	}
	const auto& texture = resource->GetTexture();
	glGenTextures(1, &mTextureHandle);
	glBindTexture(GL_TEXTURE_2D, mTextureHandle);

	// todo:  configurable
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	GLenum format;
	if(texture.channels == 1)
	{
		format = GL_RED;
	}
	else if(texture.channels == 2)
	{
		format = GL_RG;
	}
	else if(texture.channels == 3)
	{
		format = GL_RGB;
	}
	else if(texture.channels == 4)
	{
		format = GL_RGBA;
	}
	else
	{
		assert(false);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, texture.data);
	glEnable(GL_TEXTURE_2D); // intel driver bug
	glGenerateMipmap(GL_TEXTURE_2D);
}

size_t GLTexture::GetWidth() const
{
	return(mWidth);
}

size_t GLTexture::GetHeight() const
{
	return(mHeight);
}


void GLTexture::Free()
{
	glDeleteTextures(1, &mTextureHandle);
}

GLuint GLTexture::GetTextureHandle() const
{
	return(mTextureHandle);
}

GLuint GLTexture::GetTextureType() const
{
	return(mTextureType);
}

void GLTexture::Bind(size_t unit)
{
	auto glUnit = GL_TEXTURE0 + unit;
	glActiveTexture(glUnit);
	glBindTexture(GL_TEXTURE_2D, mTextureHandle);
	glActiveTexture(GL_TEXTURE0);
}
#pragma once
#include "Engine/Base/Common/Common.h"
#include <glad/glad.h>
#include <memory>

class MaterialResource;

class GLTexture
{
public:
	GLTexture(GLuint resourcePtr, size_t width, size_t height, GLuint textureType);
	GLTexture(const std::shared_ptr<MaterialResource>& resource);
	GLuint GLTexture::GetTextureHandle() const;
	void Bind(size_t unit);
	size_t GetWidth() const;
	size_t GetHeight() const;
	GLuint GetTextureType() const;

	void Free();

private:
	GLuint mTextureHandle;
	size_t mWidth;
	size_t mHeight;
	GLuint mTextureType;
};
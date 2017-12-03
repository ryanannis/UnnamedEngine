#pragma once
#include "Engine/Base/Common/Common.h"
#include <glad/glad.h>
#include <memory>

class MaterialResource;

class GLTexture
{
public:
	GLTexture(const std::shared_ptr<MaterialResource>& resource);
	void Free();
	GLuint GLTexture::GetTextureHandle() const;
	void Bind(size_t unit);

private:
	GLuint mTextureHandle;
};
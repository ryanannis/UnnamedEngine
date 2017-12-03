#include "MaterialResource.h"

#include "Engine/Base/Resource/URI.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "stb_image.h"

Texture::~Texture()
{
	stbi_image_free(data);
}

MaterialResource::MaterialResource(URI uri) :
	Resource(uri),
	mReady(false)
{
}

bool MaterialResource::IsReady() const
{
	return(mReady);
}

void MaterialResource::Load(Ptr<ResourceManager>)
{
	const auto& textureURI = GetURI();
	const auto& extension = textureURI.GetExtension();
	stbi_set_flip_vertically_on_load(true);

	t.data = stbi_load(textureURI.GetFilePath().data(), &t.width, &t.height, &t.channels, 0);
	
	if(t.data)
	{
		mReady = true;
	}
	else
	{
		mReady = false;
		std::cout << "MaterialResource failed to load: " << textureURI.GetFilePath() << std::endl;
	}
}

const Texture& MaterialResource::GetTexture() const
{
	return(t);
}

void MaterialResource::SetType(TextureType type)
{
	t.type = type;
}
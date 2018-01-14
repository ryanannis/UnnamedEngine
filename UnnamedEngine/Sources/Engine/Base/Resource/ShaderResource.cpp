#include "ShaderResource.h"

#include "Engine/Base/Resource/URI.h"
#include "Engine/Base/Resource/ResourceUtil.h"

#include <iostream>
#include <fstream>
#include <sstream>

ShaderResource::ShaderResource(URI uri) :
	Resource(uri)
{}

bool ShaderResource::IsReady() const
{
	return(mReady);
}

void ShaderResource::Load(Ptr<ResourceManager> manager)
{
	const auto& shaderURI = GetURI();
	
	const auto& extension = shaderURI.GetExtension();
	if(extension == "vert")
	{
		mShaderType = ShaderType::VERTEX;
	}
	else if(extension == "frag")
	{
		mShaderType = ShaderType::FRAGMENT;
	}
	else if(extension == "geom")
	{
		mShaderType = ShaderType::GEOMETRY;
	}
	else if(extension == "frag")
	{
		mShaderType = ShaderType::FRAGMENT;
	}
	else
	{
		assert(false);
	}

	std::string fullURL = shaderURI.GetFilePath();
	fullURL = fullURL + ".spv";

	ResourceUtil::LoadBinaryFile(fullURL);

	mReady = true;
}

const std::vector<char>& ShaderResource::GetShaderBinary() const
{
	return(mShaderBinary);
}

ShaderType ShaderResource::GetShaderType() const
{
	return(mShaderType);
}

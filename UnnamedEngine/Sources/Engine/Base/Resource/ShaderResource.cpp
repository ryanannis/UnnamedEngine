#include "ShaderResource.h"

#include "Engine/Base/Resource/URI.h"

#include <iostream>
#include <fstream>
#include <sstream>

ShaderResource::ShaderResource(std::string URI) :
	Resource(URI)
{}

bool ShaderResource::IsReady() const
{
	return(mReady);
}

void ShaderResource::Load(Ptr<ResourceManager> manager)
{
	URI shaderURI(GetURI());
	
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
	std::ifstream f(fullURL);
	std::stringstream buffer;
	buffer << f.rdbuf();
	mShaderText = buffer.str();

	mReady = true;
}

const std::string& ShaderResource::GetShaderText() const
{
	return(mShaderText);
}

ShaderType ShaderResource::GetShaderType() const
{
	return(mShaderType);
}

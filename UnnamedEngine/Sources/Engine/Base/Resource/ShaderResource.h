#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/Resource.h"

enum class ShaderType
{
	VERTEX,
	GEOMETRY,
	FRAGMENT,
	COMPUTE
};

class ShaderResource : public Resource
{
public:
	ShaderResource(URI uri);
	virtual bool IsReady() const override;
	virtual void Load(Ptr<ResourceManager> manager) override;
	const std::vector<char>& ShaderResource::GetShaderBinary() const;
	ShaderType GetShaderType() const;

private:
	std::vector<char> mShaderBinary;
	ShaderType mShaderType;
	bool mReady;
};
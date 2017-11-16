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
	ShaderResource(std::string URI);
	virtual bool IsReady() const override;
	virtual void Load(Ptr<ResourceManager> manager) override;
	const std::string& GetShaderText() const;
	ShaderType GetShaderType() const;

private:
	std::string mShaderText;
	ShaderType mShaderType;
	bool mReady;
};
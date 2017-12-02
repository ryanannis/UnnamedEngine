#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/Resource.h"

enum class TextureType
{
	DIFFUSE,
	SPECULAR
};

struct Texture
{
public:
	~Texture() { std::free(data); }
	URI filepath;
	int width;
	int height;
	int channels;
	unsigned char* data;
};

class MaterialResource : public Resource
{
public:
	MaterialResource(URI uri);
	virtual bool IsReady() const override;
	virtual void Load(Ptr<ResourceManager> manager) override;
	const Texture& GetTexture() const;

private:
	bool mReady;
	Texture t;
};
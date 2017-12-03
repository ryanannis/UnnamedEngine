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
	~Texture();
	URI filepath;
	TextureType type;
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
	virtual void Load(Ptr<ResourceManager>) override;
	const Texture& GetTexture() const;

	//todo:  set up custom data format so we don't have to do this :(
	void SetType(TextureType t);

private:
	bool mReady;
	Texture t;
};
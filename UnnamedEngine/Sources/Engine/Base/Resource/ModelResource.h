#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/ResourceType.h"

#include <vector>
#include <map>

struct Mesh
{
	std::vector<uint32_t> mIndices;
	// format vvvnnnuu
	std::vector<float> mVectors;
	std::vector<ResourceType<MaterialResource>> mDiffuseTextures;
	std::vector<ResourceType<MaterialResource>> mSpecularTextures;
};

class ModelResource : public Resource
{
public:
	ModelResource(URI URI);
	virtual bool IsReady() const override;
	virtual void Load(Ptr<ResourceManager> manager) override;
	const std::vector<Mesh>& GetMeshes() const;

private:

	std::vector<Mesh> mMeshes;
	bool mReady;
};
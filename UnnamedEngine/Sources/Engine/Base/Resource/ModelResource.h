#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Resource/Resource.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/MaterialResource.h"

#include <vector>
#include <map>
#include "Formats/MeshFormat.h"

class ModelResource : public Resource
{
public:
	ModelResource(URI URI);
	virtual ~ModelResource();
	virtual bool IsReady() const override;
	virtual void Load(Ptr<ResourceManager> manager) override;

	const bool IsStaticMesh() const;
	const MeshData& GetMeshes() const;

private:
	MeshData mData;
	bool mReady;
};
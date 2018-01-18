#pragma once

#include "Engine/Base/Resource/ModelResource.h"

#include <memory>

#include <vulkan\vulkan.h>

class VulkanMeshManager;

class VulkanModel
{
public:
	VulkanModel(std::shared_ptr<ModelResource> model);
	void Initialize(VulkanMeshManager* driver);
	void Release(VulkanMeshManager* driver);

private:
	std::shared_ptr<ModelResource> mModelResource;
};
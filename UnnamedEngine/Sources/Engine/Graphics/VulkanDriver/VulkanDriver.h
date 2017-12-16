#pragma once
#include "Engine/Base/Common/Common.h"
#include <vulkan\vulkan.h>

class ResourceManager;

class VulkanDriver
{
public:
	VulkanDriver(Ptr<ResourceManager> resourceManager);
	void Initialize();

private:
	Ptr<ResourceManager> mResourceManager;
	VkInstance instance;
};
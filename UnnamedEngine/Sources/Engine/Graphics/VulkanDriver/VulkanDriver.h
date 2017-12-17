#pragma once
#include "Engine/Base/Common/Common.h"
#include <vulkan\vulkan.h>

class ResourceManager;
struct DriverSettings
{
	// Interfacing between WM and Vulkan Driver
	char** windowManagerExtensions;
	size_t numExtensions;
};


class VulkanDriver
{
public:
	VulkanDriver(Ptr<ResourceManager> resourceManager);
	void Initialize(const DriverSettings& driverSettings);

private:
	Ptr<ResourceManager> mResourceManager;
	VkInstance mInstance;
};
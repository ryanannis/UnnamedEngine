#pragma once
#include "Engine/Base/Common/Common.h"
#include <vulkan\vulkan.h>

class ResourceManager;
struct DriverSettings
{
	bool useValidationLayers;
	// Interfacing between WM and Vulkan Driver
	char** windowManagerExtensions;
	size_t numExtensions;
};

class VulkanDriver
{
public:
	VulkanDriver(Ptr<ResourceManager> resourceManager);
	void Initialize(const DriverSettings& driverSettings);
	void Cleanup();
private:

	void SetupVulkanInstance();
	void SetupValidationLayers();
	bool CheckValidationLayerSupport();
	Ptr<ResourceManager> mResourceManager;

	// Vulkan Resources
	VkInstance mInstance;
	DriverSettings mDriverSettings;
	VkDebugReportCallbackEXT mValidationCallback;
};
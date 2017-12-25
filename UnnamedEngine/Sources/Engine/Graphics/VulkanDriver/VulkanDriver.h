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

struct QueueFamilyIndices
{
	size_t graphicsFamily = INVALID_INDEX;
};

class VulkanDriver
{
public:
	VulkanDriver(Ptr<ResourceManager> resourceManager);
	void Initialize(const DriverSettings& driverSettings);
	void Cleanup();

private:
	void SetupPhysicalDevice();
	size_t RatePhysicalDevice(VkPhysicalDevice device);
	QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice device);
	void SetupLogicalDevice();
	
	void SetupVulkanInstance();
	void SetupValidationLayers();
	bool CheckValidationLayerSupport();
	Ptr<ResourceManager> mResourceManager;

	// Vulkan Resources
	VkInstance mInstance;
	VkPhysicalDevice mPhysicalDevice;
	VkDevice mLogicalDevice;
	VkQueue mGraphicsQueue;

	DriverSettings mDriverSettings;
	VkDebugReportCallbackEXT mValidationCallback;
};
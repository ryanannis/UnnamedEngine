#pragma once
#include "Engine/Base/Common/Common.h"

#include "Engine/Graphics/VulkanDriver/VulkanApplication.h"

class VulkanDriver;

// This class has the intent of keeping the Vulkan initialization boilerplate
// out of VulkanDriver for a clean interface and compilation time reasons.
class VulkanApplicationFactory : public NonCopyable
{
public:
	VulkanApplicationFactory(VulkanDriver* driver);
	VulkanApplication CreateApplication();

private:
	VulkanDriver* mDriver;
	VulkanApplication* mApplication;  // This is set everytime CreateApplication is called.  Dirty hack to reduce the number of method args.

	// Device support
	size_t RatePhysicalDevice(VkPhysicalDevice device);
	bool CheckExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails CheckSwapChainSupport(VkPhysicalDevice device);
	
	VkPresentModeKHR SelectSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkSurfaceFormatKHR SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkExtent2D SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	
	std::vector<char*> GetRequiredInstanceExtensions();
	std::vector<char*> GetRequiredDeviceExtensions();

	QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice device);

	// Initialization
	void InitializeApplication();
	void SetupVulkanInstance();
	void SetupSurface();
	void SetupPhysicalDevice();
	void SetupLogicalDevice();
	void SetupSwapchain();
	void SetupSwapchainImageViews();
	void SetupCommandPools();
	void SetupMemoryPools();

	void SetupValidationLayers();
	bool CheckValidationLayerSupport();
};
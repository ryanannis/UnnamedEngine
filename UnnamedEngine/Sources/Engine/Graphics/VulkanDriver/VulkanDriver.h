#pragma once
#include "Engine/Base/Common/Common.h"

#include <functional>

#include <vulkan\vulkan.h>

#include "vk_mem_alloc.h"

class ResourceManager;
struct DriverSettings
{
	size_t renderWidth;
	size_t renderHeight;

	bool vSync;

	bool useValidationLayers;
	// Interfacing between WM and Vulkan Driver
	char** windowManagerExtensions;
	std::function<VkResult(VkInstance instance, const VkAllocationCallbacks* allocator, VkSurfaceKHR* surface)> windowManagerSurfaceCreationCallback;

	size_t numExtensions;
};

struct QueueFamilyIndices
{
	size_t graphicsFamily = INVALID_INDEX;
	size_t presentFamily = INVALID_INDEX;
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct VulkanVertexBuffer
{
	VmaAllocation allocation;
	VkBuffer buffer;
};

class VulkanDriver
{
public:
	VulkanDriver(Ptr<ResourceManager> resourceManager);
	void Initialize(const DriverSettings& driverSettings);
	void Cleanup();
	void DrawFrame();

private:
	void SetupPhysicalDevice();
	size_t RatePhysicalDevice(VkPhysicalDevice device);
	QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice device);
	void SetupLogicalDevice();
	void SetupSurface();
	void CreatePipeline();
	void SetupCommandBuffers();
	void RecordCommandBuffers();
	void SetupMemoryPools();
	void SetupImageViews();
	
	VulkanVertexBuffer CreateVertexBuffer(VkDeviceSize size);

	void SetupVulkanInstance();
	void SetupValidationLayers();
	bool CheckValidationLayerSupport();
	bool CheckExtensionSupport(VkPhysicalDevice device);
	
	SwapChainSupportDetails CheckSwapChainSupport(VkPhysicalDevice device);

	VkPresentModeKHR SelectSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkSurfaceFormatKHR SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkExtent2D SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	void SetupDefaultSwapchain();

	std::vector<char*> GetRequiredInstanceExtensions();
	std::vector<char*> GetRequiredDeviceExtensions();

	Ptr<ResourceManager> mResourceManager;

	// Vulkan Resources
	VkInstance mInstance;
	VkPhysicalDevice mPhysicalDevice;
	VkDevice mLogicalDevice;
	VkSurfaceKHR mSurface;

	VkSemaphore mImageAvailableSemaphore;
	VkSemaphore mFinishedRenderingSemaphore;

	VkSwapchainKHR mDefaultSwapchain;
	VkFormat mDefaultSwapchainFormat;
	VkExtent2D mDefaultSwapchainExtent;
	VmaAllocator mAllocator;
	std::vector<VkImage> mSwapChainImages;
	std::vector<VkImageView> mSwapchainImageViews;


	VkCommandPool mPresentCommandPool;

	std::vector<VkCommandBuffer> mPresentationCommandBuffers;
	QueueFamilyIndices mQueueIndices;
	VkQueue mGraphicsQueue;
	VkQueue mPresentQueue;

	DriverSettings mDriverSettings;
	VkDebugReportCallbackEXT mValidationCallback;
};
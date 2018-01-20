#pragma once
#include "Engine/Base/Common/Common.h"

#include <memory>

#include <vulkan\vulkan.h>

#include "Engine/Graphics/VulkanDriver/VulkanShaderManager.h"
#include "Engine/Graphics/VulkanDriver/VulkanMeshManager.h"

VK_DEFINE_HANDLE(VmaAllocator);
VK_DEFINE_HANDLE(VmaAllocation);

class ResourceManager;

struct SwapChainImageData
{
	VkImage image;
	VkImageView imageView;
	VkFramebuffer framebuffer;
};

struct RenderingResources
{
    VkFramebuffer framebuffer;
    VkCommandBuffer commandbuffer;
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore finishedRenderingSemaphore;
    VkFence fence;
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

struct VulkanApplication
{
	inline VkCommandPool GetGraphicsCommandPool()
	{
		return(graphicsCommandPool);
	}

	// Managers
	std::unique_ptr<VulkanMeshManager> meshManager;
	std::unique_ptr<VulkanShaderManager> shaderManager;

	// Vulkan Resources
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice logicalDevice;
	VkSurfaceKHR surface;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore finishedRenderingSemaphore;

	VkSwapchainKHR defaultSwapchain;
	VkFormat defaultSwapchainFormat;
	VkExtent2D defaultSwapchainExtent;
	VmaAllocator allocator;

	std::vector<SwapChainImageData> swapChainData;

	VkCommandPool graphicsCommandPool;
	VkCommandPool presentCommandPool;

	QueueFamilyIndices queueIndices;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	
	VkViewport viewport;
	VkRect2D viewportScissor;

	VkDebugReportCallbackEXT validationCallback;
};

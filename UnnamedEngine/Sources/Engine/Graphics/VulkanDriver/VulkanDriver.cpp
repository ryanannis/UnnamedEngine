#include "VulkanDriver.h"
#include "Engine/Base/Utility/SingletonLogger.h"

#include <GLFW/glfw3.h>
#include <set>
#include "Engine/Graphics/VulkanDriver/VulkanExtensionDefs.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

static VKAPI_ATTR VkBool32 VKAPI_CALL ValidationCallback
(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objType,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData
)
{
	SingletonLogger::Log(GFX_DRIVER_LOG, LogType::ERROR).Log(msg);

	return(VK_FALSE);
}

const std::vector<const char*> VALIDATION_LAYERS =
{
	"VK_LAYER_LUNARG_standard_validation"
};

const std::vector<char*> REQUIRED_DEVICE_EXTENSIONS =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};


VulkanDriver::VulkanDriver(Ptr<ResourceManager> resourceManager) :
	mResourceManager(resourceManager)
{
}

void VulkanDriver::Initialize(const DriverSettings& driverSettings)
{
	mDriverSettings = driverSettings;
	SetupVulkanInstance();
	if(mDriverSettings.useValidationLayers)
	{
		SetupValidationLayers();
	}
	SetupSurface();
	SetupPhysicalDevice();
	SetupLogicalDevice();
	SetupDefaultSwapchain();
	SetupCommandBuffers();
	SetupImageViews();
}

void VulkanDriver::SetupVulkanInstance()
{
	// End test
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "UnnnamedEngine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "UnnnamedEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	const auto extensions = GetRequiredInstanceExtensions();
	createInfo.enabledExtensionCount = extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();
	
	if(mDriverSettings.useValidationLayers && !CheckValidationLayerSupport())
	{
		assert(false);  // Requested validation layer not found
	}

	if(mDriverSettings.useValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	const auto instanceCode = vkCreateInstance(&createInfo, nullptr, &mInstance);
	assert(instanceCode == VK_SUCCESS);
}

/*
* A return code of 0 means the device is unsuitable.
*/
size_t VulkanDriver::RatePhysicalDevice(VkPhysicalDevice device)
{
	// todo:  interface for this to be selected in non-graphics code
	size_t score = 0;

	// Check device features and properties
	VkPhysicalDeviceProperties  deviceProperties;
	VkPhysicalDeviceFeatures	deviceFeatures;

	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	if(!deviceFeatures.geometryShader)
	{
		return(0);
	}

	if(!CheckExtensionSupport(device))
	{
		return(0);
	}

	// Ensure the physical device has a presentation queue
	QueueFamilyIndices queueIndices = GetQueueFamilyIndices(device);
	if(queueIndices.graphicsFamily == INVALID_INDEX)
	{
		return(0);
	}

	// Ensure the physical device has a presentation queue
	if(queueIndices.presentFamily == INVALID_INDEX)
	{
		return(0);
	}

	// Always pick a discrete GPU over integrated
	if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
	{
		score += 1000000;
	}

	// Secondarily, pick the one with the max texture size
	score += deviceProperties.limits.maxImageDimension2D;

	return(score);
}

QueueFamilyIndices VulkanDriver::GetQueueFamilyIndices(VkPhysicalDevice device)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	QueueFamilyIndices queueIndices;

	for(size_t i = 0; i < queueFamilies.size(); i++)
	{
		const auto& queueFamily = queueFamilies[i];
		if(queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			queueIndices.graphicsFamily = i;
		}
		
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface, &presentSupport);

		if(queueFamily.queueCount > 0 && presentSupport)
		{
			queueIndices.presentFamily = i;
		}

		if(queueIndices.graphicsFamily != INVALID_INDEX && queueIndices.presentFamily != INVALID_INDEX)
		{
			break;
		}
	}

	return(queueIndices);
}

void VulkanDriver::SetupLogicalDevice()
{
	auto indices = GetQueueFamilyIndices(mPhysicalDevice);
	mQueueIndices = indices;

	const float queuePriority = 1.f;

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
	queueCreateInfo.queueCount = 1; //todo: multiple graphics queues
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;

	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;

	const auto deviceExtensions = GetRequiredDeviceExtensions();
	createInfo.enabledExtensionCount = deviceExtensions.size();
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if(mDriverSettings.useValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	assert(vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mLogicalDevice) == VK_SUCCESS);

	vkGetDeviceQueue(mLogicalDevice, indices.graphicsFamily, 0, &mGraphicsQueue);
	vkGetDeviceQueue(mLogicalDevice, indices.presentFamily, 0, &mPresentQueue);
}

void VulkanDriver::SetupSurface()
{
	auto retCode = mDriverSettings.windowManagerSurfaceCreationCallback(mInstance, nullptr, &mSurface);
	assert(retCode == VK_SUCCESS);
}

void VulkanDriver::SetupImageViews()
{
	mSwapchainImageViews.resize(mSwapChainImages.size());
	for(size_t i = 0; i < mSwapChainImages.size(); i++)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = mSwapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = mDefaultSwapchainFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		assert(vkCreateImageView(mLogicalDevice, &createInfo, nullptr, &mSwapchainImageViews[i]) == VK_SUCCESS);
	}
}

void VulkanDriver::SetupMemoryPools()
{
	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.physicalDevice = mPhysicalDevice;
	allocatorInfo.device = mLogicalDevice;

	vmaCreateAllocator(&allocatorInfo, &mAllocator);
}

void VulkanDriver::SetupCommandBuffers()
{
	VkCommandPoolCreateInfo commandPoolCreateInfo;
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = mQueueIndices.presentFamily;

	assert(vkCreateCommandPool(mLogicalDevice, &commandPoolCreateInfo, nullptr, &mPresentCommandPool) == VK_SUCCESS);
	
	const uint32_t numImages = mSwapChainImages.size();
	mPresentationCommandBuffers.resize(numImages);

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = mPresentCommandPool;
	commandBufferAllocateInfo.commandPool = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = numImages;

	assert(vkAllocateCommandBuffers(mLogicalDevice, &commandBufferAllocateInfo, mPresentationCommandBuffers.data()) == VK_SUCCESS);

	RecordCommandBuffers();
}

void VulkanDriver::RecordCommandBuffers()
{
	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

	VkClearColorValue clearColor = { 1.0f, 0.0f, 0.0f, 0.0f };
	
	VkImageSubresourceRange imageSubresourceRange;
	imageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageSubresourceRange.baseMipLevel = 0;
	imageSubresourceRange.levelCount = 1;
	imageSubresourceRange.baseArrayLayer = 0;
	imageSubresourceRange.layerCount = 1;
}

VulkanVertexBuffer VulkanDriver::CreateVertexBuffer(VkDeviceSize size)
{
	VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.size = size;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	
	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	
	VulkanVertexBuffer vertexBuffer;

	vmaCreateBuffer(
		mAllocator, 
		&bufferInfo, 
		&allocInfo, 
		&vertexBuffer.buffer, 
		&vertexBuffer.allocation, 
		nullptr
	);
	
	return(vertexBuffer);
}

void VulkanDriver::CreatePipeline()
{
}

void VulkanDriver::SetupPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);

	assert(deviceCount > 0); // no device

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

	size_t bestScore = 0;
	VkPhysicalDevice bestDevice;

	for(const auto& device : devices)
	{
		const size_t deviceScore = RatePhysicalDevice(device);
		if(deviceScore >= bestScore)
		{
			bestDevice = device;
			bestScore = deviceScore;
		}
	}

	assert(bestScore > 0); // no compatible physical device
	mPhysicalDevice = bestDevice;
}

void VulkanDriver::SetupValidationLayers()
{
	/// Setup validation layer callback
	VkDebugReportCallbackCreateInfoEXT callbackCreateInfo = {};
	callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	callbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	callbackCreateInfo.pfnCallback = ValidationCallback;

	if(CreateDebugReportCallbackEXT(mInstance, &callbackCreateInfo, nullptr, &mValidationCallback) != VK_SUCCESS)
	{
		assert(false);
	}
}

bool VulkanDriver::CheckValidationLayerSupport()
{
	// Checks if we have the requested validation layers
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for(const char* layerName : VALIDATION_LAYERS)
	{
		bool layerFound = false;

		for(const auto& layerProperties : availableLayers)
		{
			if(strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if(!layerFound)
		{
			return(false);
		}
	}

	return(true);
}

bool VulkanDriver::CheckExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	const auto deviceExtensions = GetRequiredDeviceExtensions();
	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for(const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return(requiredExtensions.empty());
}

SwapChainSupportDetails VulkanDriver::CheckSwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface, &details.capabilities);

	uint32_t numFormats;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &numFormats, nullptr);
	if(numFormats > 0)
	{
		details.formats.resize(numFormats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &numFormats, details.formats.data());
	}

	uint32_t numPresentModes;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &numPresentModes, nullptr);
	if(numPresentModes > 0)
	{
		details.presentModes.resize(numPresentModes);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &numPresentModes, details.presentModes.data());
	}

	return(details);
}

VkPresentModeKHR VulkanDriver::SelectSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
	//todo:  vsync mDriverSettings options
	if(mDriverSettings.vSync)
	{
		if(std::find(availablePresentModes.begin(), availablePresentModes.end(), VK_PRESENT_MODE_FIFO_KHR) == availablePresentModes.end())
		{
			SingletonLogger::Log(GFX_DRIVER_LOG, LogType::ERROR).Log(
				"Attempted to use VSync but device does not support double buffering!  Defaulting to immediate mode."
			);
			return(VK_PRESENT_MODE_IMMEDIATE_KHR);
		}
		return(VK_PRESENT_MODE_FIFO_KHR);
	}
	else
	{
		return(VK_PRESENT_MODE_IMMEDIATE_KHR);
	}
}

VkSurfaceFormatKHR VulkanDriver::SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	if(availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED)
	{
		return(VkSurfaceFormatKHR{VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR});
	}

	for(const auto& availableFormat : availableFormats)
	{
		if
		(
			availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && 
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
		)
		{
			return(availableFormat);
		}
	}

	assert(false);
	return(availableFormats[0]);
}

VkExtent2D VulkanDriver::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return(capabilities.currentExtent);
	}
	else
	{
		VkExtent2D actualExtent = VkExtent2D({mDriverSettings.renderWidth, mDriverSettings.renderHeight});

		actualExtent.width = std::max(
			capabilities.minImageExtent.width, 
			std::min(capabilities.maxImageExtent.width, actualExtent.width)
		);
		actualExtent.height = std::max(
			capabilities.minImageExtent.height, 
			std::min(capabilities.maxImageExtent.height, actualExtent.height)
		);

		return(actualExtent);
	}
}

void VulkanDriver::SetupDefaultSwapchain()
{
	SwapChainSupportDetails swapChainSupport = CheckSwapChainSupport(mPhysicalDevice);
	assert(!swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty());

	const VkSurfaceFormatKHR surfaceFormat = SelectSwapSurfaceFormat(swapChainSupport.formats);
	const VkPresentModeKHR presentMode = SelectSwapPresentMode(swapChainSupport.presentModes);
	const VkExtent2D extent = SelectSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount;
	if(imageCount == 0)
	{
		imageCount = 1;
	}

	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.flags = 0;
	semaphoreCreateInfo.pNext = nullptr;
	assert(vkCreateSemaphore(mLogicalDevice, &semaphoreCreateInfo, nullptr, &mImageAvailableSemaphore) == VK_SUCCESS);
	assert(vkCreateSemaphore(mLogicalDevice, &semaphoreCreateInfo, nullptr, &mFinishedRenderingSemaphore) == VK_SUCCESS);

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = mSurface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = GetQueueFamilyIndices(mPhysicalDevice);
	uint32_t queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

	if(indices.graphicsFamily != indices.presentFamily)
	{
		//todo:  explicit ownership transfer for doing MT
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	assert(vkCreateSwapchainKHR(mLogicalDevice, &createInfo, nullptr, &mDefaultSwapchain) == VK_SUCCESS);

	uint32_t swapchainImageCount;
	vkGetSwapchainImagesKHR(mLogicalDevice, mDefaultSwapchain, &swapchainImageCount, nullptr);
	mSwapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(mLogicalDevice, mDefaultSwapchain, &swapchainImageCount, mSwapChainImages.data());

	mDefaultSwapchainFormat = surfaceFormat.format;
	mDefaultSwapchainExtent = extent;
}

std::vector<char*> VulkanDriver::GetRequiredInstanceExtensions()
{
	// Add required window manager extensions
	std::vector<char*> extensions;
	for(size_t i = 0; i < mDriverSettings.numExtensions; i++)
	{
		extensions.push_back(mDriverSettings.windowManagerExtensions[i]);
	}

	if(mDriverSettings.useValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return(extensions);
}

std::vector<char*> VulkanDriver::GetRequiredDeviceExtensions()
{
	std::vector<char*> extensions;
	for(size_t i = 0; i < REQUIRED_DEVICE_EXTENSIONS.size(); i++)
	{
		extensions.push_back(REQUIRED_DEVICE_EXTENSIONS[i]);
	}

	return(extensions);
}

void VulkanDriver::Cleanup()
{
	for(auto imageView : mSwapchainImageViews) {
		vkDestroyImageView(mLogicalDevice, imageView, nullptr);
	}

	vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
	vkDestroySwapchainKHR(mLogicalDevice, mDefaultSwapchain, nullptr);
	vkDestroyDevice(mLogicalDevice, nullptr);
	vkDestroyInstance(mInstance, nullptr);
}

void VulkanDriver::DrawFrame()
{
	// Acquire image
	uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
		mLogicalDevice, 
		mDefaultSwapchain, 
		std::numeric_limits<uint64_t>::max(), // todo: potentially blocking thread forever is a bit...
		mImageAvailableSemaphore, 
		VK_NULL_HANDLE, 
		&imageIndex
	);

    if(result == VK_ERROR_OUT_OF_DATE_KHR){
		assert(false); // todo:  recreate swapchain
        return;
    }
	else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		assert(false);
    }

	VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &mImageAvailableSemaphore;
	submitInfo.pWaitDstStageMask = &waitFlags;
	submitInfo.commandBufferCount = 1;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &mFinishedRenderingSemaphore;

	assert(vkQueueSubmit(mPresentQueue, 1, &submitInfo, VK_NULL_HANDLE) == VK_SUCCESS);

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &mFinishedRenderingSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &mDefaultSwapchain;
	presentInfo.pImageIndices = &imageIndex;

	assert(vkQueuePresentKHR(mPresentQueue, &presentInfo));
}

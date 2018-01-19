#include "VulkanApplicationFactory.h""

#include <set>

#include "Engine/Base/Utility/SingletonLogger.h"

#include "Engine/Graphics/VulkanDriver/VulkanDriver.h"
#include "Engine/Graphics/VulkanDriver/VulkanExtensionDefs.h"

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

VulkanApplicationFactory::VulkanApplicationFactory(VulkanDriver* driver) :
	mDriver(driver)
{}

VulkanApplication VulkanApplicationFactory::CreateApplication()
{
	VulkanApplication application;
	mApplication = &application;
	InitializeApplication();
	return(application);
}

void VulkanApplicationFactory::SetupVulkanInstance()
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
	
	if(mDriver->GetDriverSettings().useValidationLayers && !CheckValidationLayerSupport())
	{
		assert(false);  // Requested validation layer not found
	}

	if(mDriver->GetDriverSettings().useValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	const auto instanceCode = vkCreateInstance(&createInfo, nullptr, &mApplication->instance);
	assert(instanceCode == VK_SUCCESS);
}

/*
* A return code of 0 means the device is unsuitable.
*/
size_t VulkanApplicationFactory::RatePhysicalDevice(VkPhysicalDevice device)
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

QueueFamilyIndices VulkanApplicationFactory::GetQueueFamilyIndices(VkPhysicalDevice device)
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
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mApplication->surface, &presentSupport);

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

void VulkanApplicationFactory::SetupLogicalDevice()
{
	auto indices = GetQueueFamilyIndices(mApplication->physicalDevice);
	mApplication->queueIndices = indices;

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

	if(mDriver->GetDriverSettings().useValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(VALIDATION_LAYERS.size());
		createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if(vkCreateDevice(mApplication->physicalDevice, &createInfo, nullptr, &mApplication->logicalDevice) != VK_SUCCESS)
	{
		assert(false);
	}

	vkGetDeviceQueue(mApplication->logicalDevice, indices.graphicsFamily, 0, &mApplication->graphicsQueue);
	vkGetDeviceQueue(mApplication->logicalDevice, indices.presentFamily, 0, &mApplication->presentQueue);
}

void VulkanApplicationFactory::SetupSurface()
{
	auto retCode = mDriver->GetDriverSettings().windowManagerSurfaceCreationCallback(mApplication->instance, nullptr, &mApplication->surface);
	assert(retCode == VK_SUCCESS);
}

void VulkanApplicationFactory::SetupSwapchainImageViews()
{
	mApplication->swapchainImageViews.resize(mApplication->swapChainImages.size());
	for(size_t i = 0; i < mApplication->swapChainImages.size(); i++)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = mApplication->swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = mApplication->defaultSwapchainFormat;

		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if(
			vkCreateImageView(
				mApplication->logicalDevice,
				&createInfo, nullptr,
				&mApplication->swapchainImageViews[i]
			) != VK_SUCCESS)
		{
			assert(false);
		}
	}
}

void VulkanApplicationFactory::SetupCommandPools()
{
	// At present graphics is not multithreaded so there is only a single non-locked command pool sitting on the main thread...
	VkCommandPoolCreateInfo commandPoolCreateInfo;
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
	commandPoolCreateInfo.queueFamilyIndex = mApplication->queueIndices.presentFamily;

	if(vkCreateCommandPool(mApplication->logicalDevice, &commandPoolCreateInfo, nullptr, &mApplication->presentCommandPool) != VK_SUCCESS)
	{
		assert(false);
	}
}

void VulkanApplicationFactory::SetupMemoryPools()
{
	VmaAllocatorCreateInfo allocatorInfo = {};
	allocatorInfo.physicalDevice = mApplication->physicalDevice;
	allocatorInfo.device = mApplication->logicalDevice;

	vmaCreateAllocator(&allocatorInfo, &mApplication->allocator);
}

void VulkanApplicationFactory::SetupCommandBuffers()
{
	const uint32_t numImages = mApplication->swapChainImages.size();
	mApplication->presentationCommandBuffers.resize(numImages);

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandPool = mApplication->presentCommandPool;
	commandBufferAllocateInfo.commandPool = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandBufferCount = numImages;

	if(vkAllocateCommandBuffers(
		mApplication->logicalDevice, 
		&commandBufferAllocateInfo, 
		mApplication->presentationCommandBuffers.data()
	) != VK_SUCCESS)
	{
		assert(false);
	}
}

VulkanVertexBuffer VulkanApplicationFactory::CreateVertexBuffer(VkDeviceSize size)
{
	VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.size = size;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
	
	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
	
	VulkanVertexBuffer vertexBuffer;

	vmaCreateBuffer(
		mApplication->allocator, 
		&bufferInfo, 
		&allocInfo, 
		&vertexBuffer.buffer, 
		&vertexBuffer.allocation, 
		nullptr
	);
	
	return(vertexBuffer);
}

void VulkanApplicationFactory::InitializeApplication()
{
	InitializeManagers();
	SetupSurface();
	SetupPhysicalDevice();
	SetupLogicalDevice();
	SetupDefaultSwapchain();
	SetupCommandBuffers();
	SetupSwapchainImageViews();
	SetupMemoryPools();
	SetupCommandPools();
}

void VulkanApplicationFactory::InitializeManagers()
{
	mApplication->shaderManager = std::make_unique<VulkanShaderManager>(mDriver, mDriver->GetResourceManager());
	mApplication->meshManager = std::make_unique<VulkanMeshManager>(mDriver, mDriver->GetResourceManager());
}

void VulkanApplicationFactory::SetupPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(mApplication->instance, &deviceCount, nullptr);

	assert(deviceCount > 0); // no device

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(mApplication->instance, &deviceCount, devices.data());

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
	mApplication->physicalDevice = bestDevice;
}

void VulkanApplicationFactory::SetupValidationLayers()
{
	/// Setup validation layer callback
	VkDebugReportCallbackCreateInfoEXT callbackCreateInfo = {};
	callbackCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	callbackCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	callbackCreateInfo.pfnCallback = ValidationCallback;

	if(CreateDebugReportCallbackEXT(mApplication->instance, &callbackCreateInfo, nullptr, &mApplication->validationCallback) != VK_SUCCESS)
	{
		assert(false);
	}
}

bool VulkanApplicationFactory::CheckValidationLayerSupport()
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

bool VulkanApplicationFactory::CheckExtensionSupport(VkPhysicalDevice device)
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

SwapChainSupportDetails VulkanApplicationFactory::CheckSwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mApplication->surface, &details.capabilities);

	uint32_t numFormats;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, mApplication->surface, &numFormats, nullptr);
	if(numFormats > 0)
	{
		details.formats.resize(numFormats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, mApplication->surface, &numFormats, details.formats.data());
	}

	uint32_t numPresentModes;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, mApplication->surface, &numPresentModes, nullptr);
	if(numPresentModes > 0)
	{
		details.presentModes.resize(numPresentModes);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, mApplication->surface, &numPresentModes, details.presentModes.data());
	}

	return(details);
}

VkPresentModeKHR VulkanApplicationFactory::SelectSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes)
{
	//todo:  vsync mDriver->GetDriverSettings() options
	if(mDriver->GetDriverSettings().vSync)
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

VkSurfaceFormatKHR VulkanApplicationFactory::SelectSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
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

VkExtent2D VulkanApplicationFactory::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
	{
		return(capabilities.currentExtent);
	}
	else
	{
		VkExtent2D actualExtent = VkExtent2D({mDriver->GetDriverSettings().renderWidth, mDriver->GetDriverSettings().renderHeight});

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

void VulkanApplicationFactory::SetupDefaultSwapchain()
{
	SwapChainSupportDetails swapChainSupport = CheckSwapChainSupport(mApplication->physicalDevice);
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
	
	if(vkCreateSemaphore(mApplication->logicalDevice, &semaphoreCreateInfo, nullptr, &mApplication->imageAvailableSemaphore) != VK_SUCCESS)
	{
		assert(false);
	}

	if(vkCreateSemaphore(mApplication->logicalDevice, &semaphoreCreateInfo, nullptr, &mApplication->imageAvailableSemaphore) != VK_SUCCESS)
	{
		assert(false);
	}

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = mApplication->surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = GetQueueFamilyIndices(mApplication->physicalDevice);
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

	if(vkCreateSwapchainKHR(mApplication->logicalDevice, &createInfo, nullptr, &mApplication->defaultSwapchain) != VK_SUCCESS)
	{
		assert(false);
	}

	uint32_t swapchainImageCount;
	vkGetSwapchainImagesKHR(mApplication->logicalDevice, mApplication->defaultSwapchain, &swapchainImageCount, nullptr);
	mApplication->swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(mApplication->logicalDevice, mApplication->defaultSwapchain, &swapchainImageCount, mApplication->swapChainImages.data());

	mApplication->defaultSwapchainFormat = surfaceFormat.format;
	mApplication->defaultSwapchainExtent = extent;
}

std::vector<char*> VulkanApplicationFactory::GetRequiredInstanceExtensions()
{
	// Add required window manager extensions
	std::vector<char*> extensions;
	for(size_t i = 0; i < mDriver->GetDriverSettings().numExtensions; i++)
	{
		extensions.push_back(mDriver->GetDriverSettings().windowManagerExtensions[i]);
	}

	if(mDriver->GetDriverSettings().useValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return(extensions);
}

std::vector<char*> VulkanApplicationFactory::GetRequiredDeviceExtensions()
{
	std::vector<char*> extensions;
	for(size_t i = 0; i < REQUIRED_DEVICE_EXTENSIONS.size(); i++)
	{
		extensions.push_back(REQUIRED_DEVICE_EXTENSIONS[i]);
	}

	return(extensions);
}
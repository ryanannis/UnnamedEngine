#include "VulkanDriver.h"
#include "Engine/Base/Utility/SingletonLogger.h"

#include "Engine/Graphics/VulkanDriver/VulkanExtensionDefs.h"

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

const std::vector<const char*> validationLayers =
{
	"VK_LAYER_LUNARG_standard_validation"
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
	SetupPhysicalDevice();
	SetupLogicalDevice();
}

void VulkanDriver::SetupVulkanInstance()
{
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

	createInfo.enabledExtensionCount = extensions.size();
	createInfo.ppEnabledExtensionNames = extensions.data();
	
	if(mDriverSettings.useValidationLayers && !CheckValidationLayerSupport())
	{
		assert(false);  // Requested validation layer not found
	}

	if(mDriverSettings.useValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	if(vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS)
	{
		assert(false);
	}
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

	// Ensure the card has a graphics queue
	QueueFamilyIndices queueIndices = GetQueueFamilyIndices(device);
	if(queueIndices.graphicsFamily == INVALID_INDEX)
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
	}

	return(queueIndices);
}

void VulkanDriver::SetupLogicalDevice()
{
	auto indices = GetQueueFamilyIndices(mPhysicalDevice);

	const float queuePriority = 1.f;

	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &deviceFeatures;

	if(mDriverSettings.useValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	assert(vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mLogicalDevice) == VK_SUCCESS);

	vkGetDeviceQueue(mLogicalDevice, indices.graphicsFamily, 0, &mGraphicsQueue);
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

	for(const char* layerName : validationLayers)
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

void VulkanDriver::Cleanup()
{
	vkDestroyInstance(mInstance, nullptr);
	vkDestroyDevice(mLogicalDevice, nullptr);
}
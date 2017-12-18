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
	void* userData)
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
}
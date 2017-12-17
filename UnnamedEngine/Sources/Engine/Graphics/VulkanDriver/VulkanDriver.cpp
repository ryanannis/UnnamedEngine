#include "VulkanDriver.h"

VulkanDriver::VulkanDriver(Ptr<ResourceManager> resourceManager) :
	mResourceManager(resourceManager)
{
}

void VulkanDriver::Initialize(const DriverSettings& driverSettings)
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

	createInfo.enabledExtensionCount = driverSettings.numExtensions;
	createInfo.ppEnabledExtensionNames = driverSettings.windowManagerExtensions;
	
	const auto instanceCode = vkCreateInstance(&createInfo, nullptr, &mInstance);
	if(instanceCode == VK_SUCCESS)
	{
	}
	else
	{
		assert(false);
	}
}

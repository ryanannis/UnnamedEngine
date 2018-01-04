#pragma once

class VulkanDriver;

#include "Engine/Base/Common/Common.h"
#include <vulkan\vulkan.h>

/* Todo:  Move all the initialization shi out of VulkanDriver into here and use it as an interface.*/
class VulkanInstanceManager
{
public:
	VulkanInstanceManager(VulkanDriver* driver);

private:
	VulkanDriver* mDriver;
	VkInstance mInstance;
};
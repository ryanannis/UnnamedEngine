#pragma once
#include "Engine/Base/Common/Common.h"
#include <vulkan\vulkan.h>
#include <functional>

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

class VulkanDriver
{
public:
	VulkanDriver();
	void Initialize(DriverSettings settings);
};
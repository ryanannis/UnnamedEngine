#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Graphics/VulkanDriver/VulkanApplication.h"

#include <functional>

#include <vulkan\vulkan.h>

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
	VulkanDriver(Ptr<ResourceManager> resourceManager);
	void Initialize(const DriverSettings& driverSettings);
	const DriverSettings& GetDriverSettings();
	void Cleanup();
	void DrawFrame();

	// Temp 
	void CreatePipeline();
	void CreateShaderModules();
	void CreateFramebuffers();
	void CreateRenderPass();


private:
	VulkanApplication mApplication;
	Ptr<ResourceManager> mResourceManager;
	DriverSettings mDriverSettings;

	VkShaderModule mVertexShader;
	VkShaderModule mFragmentShader;
	std::vector<VkFramebuffer> mSwapchainFramebuffers;
	VkRenderPass mTempRenderPass;
};
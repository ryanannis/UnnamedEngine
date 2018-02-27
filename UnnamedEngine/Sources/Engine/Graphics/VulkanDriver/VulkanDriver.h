#pragma once
#include "Engine/Base/Common/Common.h"
#include "Engine/Graphics/VulkanDriver/VulkanApplication.h"
#include "Engine/Graphics/VulkanDriver/VulkanUtils.h"

#include <functional>

#include <vulkan\vulkan.h>

class ResourceManager;

struct SubmeshRenderGroup
{
	std::vector<SubmeshData *> submeshesToRender;
	VulkanUtils::Mesh::SubmeshBindingDescription bindingDescripion;
};

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

/*
 * Data used for the process of a graphics queue building and submission.
 */
struct RenderData
{
	VkImage image;
	VkFramebuffer framebuffer;
    VkCommandBuffer commandBuffer;
    VkSemaphore imageAvailableSemaphore;
	VkSemaphore finishedRenderingSemaphore;
};

class VulkanDriver
{
public:
	VulkanDriver(Ptr<ResourceManager> resourceManager);
	void Initialize(const DriverSettings& driverSettings);

	const DriverSettings& GetDriverSettings();
	Ptr<ResourceManager> GetResourceManager();

	void Cleanup();
	RenderData BuildRenderData(uint32_t swapChainIndex);
	void PrepareFrameCommandBuffer(const RenderData& r);
	void DrawFrame();
	void RenderGeometry(VkCommandBuffer command);

	// Temp 
	VkPipelineLayout CreatePipelineLayout();
	void CreatePipeline(VulkanUtils::Mesh::SubmeshBindingDescription bindingDescription);
	VkFramebuffer CreateFramebuffer(VkImageView imageView);
	void CreateRenderPass();
	void RenderMesh(ResourceType<ModelResource> m);

private:
	VulkanApplication mApplication;
	Ptr<ResourceManager> mResourceManager;
	DriverSettings mDriverSettings;

	std::vector<VkFramebuffer> mSwapchainFramebuffers;
	VkRenderPass mTempRenderPass;
	VkPipeline mTempGraphicsPipeline;
};
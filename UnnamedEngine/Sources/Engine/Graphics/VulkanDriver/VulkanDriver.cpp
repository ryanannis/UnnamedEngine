#include "VulkanDriver.h"
#include "Engine/Base/Utility/SingletonLogger.h"
#include "Engine/Graphics/VulkanDriver/VulkanApplicationFactory.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/ShaderResource.h"

#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.h"

VulkanDriver::VulkanDriver(Ptr<ResourceManager> resourceManager) :
	mResourceManager(resourceManager)
{
}

const DriverSettings& VulkanDriver::GetDriverSettings()
{
	return(mDriverSettings);
}

void VulkanDriver::Initialize(const DriverSettings& driverSettings)
{
	mDriverSettings = driverSettings;

	VulkanApplicationFactory applicationFactory(this);
	mApplication = std::move(applicationFactory.CreateApplication());
}

void VulkanDriver::CreateRenderPass()
{
	VkAttachmentDescription attachmentDescription;
	attachmentDescription.flags = 0;
	attachmentDescription.format = mApplication.defaultSwapchainFormat;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachments;
	colorAttachments.attachment = 0;
	colorAttachments.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass;
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = nullptr;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachments;
	subpass.pResolveAttachments = nullptr;
	subpass.pDepthStencilAttachment = nullptr;
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments = nullptr;

	VkRenderPassCreateInfo renderPassCreateInfo;
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &attachmentDescription;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = 0;
	renderPassCreateInfo.pDependencies = nullptr;



	if(
		vkCreateRenderPass(
			mApplication.logicalDevice,
			&renderPassCreateInfo,
			nullptr,
			&mTempRenderPass
		) != VK_SUCCESS
	)
	{
		assert(false);
	}

		
}

void VulkanDriver::CreatePipeline()
{

}

void VulkanDriver::CreateShaderModules()
{
	const static ResourceType<ShaderResource> basicVertexShader("Engine/Basic.vert");
	const static ResourceType<ShaderResource> basicFragmentShader("Engine/Basic.frag");
	const auto vertCode = mResourceManager->LoadResource(basicVertexShader)->GetShaderBinary();
	const auto fragCode = mResourceManager->LoadResource(basicFragmentShader)->GetShaderBinary();

	VkShaderModuleCreateInfo vertShaderModule;
	vertShaderModule.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vertShaderModule.codeSize = vertCode.size();
	vertShaderModule.pCode = reinterpret_cast<const uint32_t*>(vertCode.data());

	if(vkCreateShaderModule(
		mApplication.logicalDevice,
		&vertShaderModule,
		nullptr,
		&mVertexShader
	));

	VkShaderModuleCreateInfo fragShaderModule;
	fragShaderModule.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	fragShaderModule.codeSize = fragCode.size();
	fragShaderModule.pCode = reinterpret_cast<const uint32_t*>(fragCode.data());

	VkShaderModule shaderModule;

	if(vkCreateShaderModule(
		mApplication.logicalDevice,
		&fragShaderModule,
		nullptr,
		&mFragmentShader
	));
}

void VulkanDriver::CreateFramebuffers()
{
	mSwapchainFramebuffers.resize(mApplication.swapchainImageViews.size());
	for(uint32_t i = 0; i < mApplication.swapchainImageViews.size(); i++)
	{
		VkFramebufferCreateInfo framebufferCreateInfo;
		framebufferCreateInfo.renderPass = mTempRenderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = &mApplication.swapchainImageViews[i];
		framebufferCreateInfo.width = GetDriverSettings().renderWidth;
		framebufferCreateInfo.height = GetDriverSettings().renderHeight;
		framebufferCreateInfo.layers = 1;
		
		if(vkCreateFramebuffer(
				mApplication.logicalDevice,
				&framebufferCreateInfo,
				nullptr,
				&mSwapchainFramebuffers[i]
			) != VK_SUCCESS)
		{
			assert(false);
		}
	}
}

void VulkanDriver::Cleanup()
{
	for(auto imageView : mApplication.swapchainImageViews) {
		vkDestroyImageView(mApplication.logicalDevice, imageView, nullptr);
	}

	vkDestroySurfaceKHR(mApplication.instance, mApplication.surface, nullptr);
	vkDestroySwapchainKHR(mApplication.logicalDevice, mApplication.defaultSwapchain, nullptr);
	vkDestroyDevice(mApplication.logicalDevice, nullptr);
	vkDestroyInstance(mApplication.instance, nullptr);
}

void VulkanDriver::DrawFrame()
{
	// Acquire image
	uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
		mApplication.logicalDevice, 
		mApplication.defaultSwapchain, 
		std::numeric_limits<uint64_t>::max(), // todo: potentially blocking thread forever is a bit...
		mApplication.imageAvailableSemaphore, 
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
	submitInfo.pWaitSemaphores = &mApplication.imageAvailableSemaphore;
	submitInfo.pWaitDstStageMask = &waitFlags;
	submitInfo.commandBufferCount = 1;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &mApplication.imageAvailableSemaphore;

	if(vkQueueSubmit(mApplication.presentQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
	{
		assert(false);
	}

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &mApplication.finishedRenderingSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &mApplication.defaultSwapchain;
	presentInfo.pImageIndices = &imageIndex;

	if(vkQueuePresentKHR(mApplication.presentQueue, &presentInfo) != VK_SUCCESS)
	{
		assert(false);
	}
}

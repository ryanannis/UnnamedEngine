#include "VulkanDriver.h"
#include "Engine/Base/Utility/SingletonLogger.h"
#include "Engine/Graphics/VulkanDriver/VulkanApplicationFactory.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/ShaderResource.h"
#include "Engine/Graphics/VulkanDriver/VulkanUtils.h"
#include "Engine/Graphics/VulkanDriver/VulkanInitializers.h"
#include "Engine/Graphics/VulkanDriver/VulkanEngineResources.h"

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

Ptr<ResourceManager> VulkanDriver::GetResourceManager()
{
	return(mResourceManager);
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

	VkSubpassDependency preRenderPassColorAttachmentDependency;
	preRenderPassColorAttachmentDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	preRenderPassColorAttachmentDependency.dstSubpass = 0;
	preRenderPassColorAttachmentDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	preRenderPassColorAttachmentDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	preRenderPassColorAttachmentDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	preRenderPassColorAttachmentDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	preRenderPassColorAttachmentDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	VkSubpassDependency postRenderPassColorAttachmentDependency;
	postRenderPassColorAttachmentDependency.srcSubpass = 0;
	postRenderPassColorAttachmentDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
	postRenderPassColorAttachmentDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	postRenderPassColorAttachmentDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
	postRenderPassColorAttachmentDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	postRenderPassColorAttachmentDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	postRenderPassColorAttachmentDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

	const uint32_t numSubpassDependencies = 2;
	VkSubpassDependency subpassDependencies[2] = {
		preRenderPassColorAttachmentDependency,
		postRenderPassColorAttachmentDependency
	};

	VkRenderPassCreateInfo renderPassCreateInfo;
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &attachmentDescription;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = numSubpassDependencies;
	renderPassCreateInfo.pDependencies = &subpassDependencies[0];

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

VkPipelineLayout VulkanDriver::CreatePipelineLayout()
{	
	VkPipelineLayoutCreateInfo layoutCreateInfo = VulkanInitalizers::vkPipelineLayoutCreateInfo();
	layoutCreateInfo.setLayoutCount = 0;
	layoutCreateInfo.pSetLayouts = nullptr;
	layoutCreateInfo.pushConstantRangeCount = 0;
	layoutCreateInfo.pPushConstantRanges = nullptr;

	VkPipelineLayout pipelineLayout;
	// todo:  WILL LEAK!
	if(vkCreatePipelineLayout(
		mApplication.logicalDevice,
		&layoutCreateInfo, 
		nullptr, 
		&pipelineLayout
	) != VK_SUCCESS)
	{
		assert(false);
	}

	return(pipelineLayout);
}

void VulkanDriver::CreatePipeline()
{
	auto pipelineLayout = CreatePipelineLayout();

	// Prepare shaders
	
	SubmeshData* submesh = nullptr;

	ShaderHandle vertShader = mApplication.shaderManager->CreateShaderModule(VulkanEngineResources::basicVertexShader);
	ShaderHandle fragShader = mApplication.shaderManager->CreateShaderModule(VulkanEngineResources::basicFragmentShader);

	VkPipelineShaderStageCreateInfo shaderStages[2] = {
		mApplication.shaderManager->GetShaderPipelineInfo(vertShader),
		mApplication.shaderManager->GetShaderPipelineInfo(fragShader)
	};

	const auto submeshAttributes = VulkanUtils::Mesh::ComputeSubmeshBindingDescription(submesh);
	
	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = 
		VulkanInitalizers::vkPipelineVertexInputStateCreateInfo();
	vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
	vertexInputStateCreateInfo.pVertexBindingDescriptions = &submeshAttributes.vertexBinding;
	vertexInputStateCreateInfo.vertexAttributeDescriptionCount = submeshAttributes.attributeBindings.size();
	vertexInputStateCreateInfo.pVertexAttributeDescriptions = &submeshAttributes.attributeBindings[0];

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = 
		VulkanInitalizers::vkPipelineInputAssemblyStateCreateInfo(
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VK_FALSE,
			0
		);

	VkViewport viewport = VulkanInitalizers::vkViewport(
		GetDriverSettings().renderWidth,
		GetDriverSettings().renderHeight
	);
	
	VkRect2D viewportScissor = VulkanInitalizers::vkRect2DScissor(
		GetDriverSettings().renderWidth,
		GetDriverSettings().renderHeight
	);

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo = 
		VulkanInitalizers::vkPipelineViewportStateCreateInfo(1);
	
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.pScissors = &viewportScissor;

	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = 
		VulkanInitalizers::vkPipelineRasterizationStateCreateInfo(
			VK_POLYGON_MODE_FILL,
			VK_CULL_MODE_BACK_BIT,
			VK_FRONT_FACE_COUNTER_CLOCKWISE
		);

	VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo =
		VulkanInitalizers::vkPipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);
	multisamplingStateCreateInfo.minSampleShading = 1.f;

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState =
		VulkanInitalizers::vkPipelineColorBlendAttachmentState(
			false,
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
		);

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo =
		VulkanInitalizers::vkPipelineColorBlendStateCreateInfo();
	colorBlendStateCreateInfo.attachmentCount = 1;
	colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
	colorBlendStateCreateInfo.blendConstants[0] = colorBlendStateCreateInfo.blendConstants[1] = 0.f;
	colorBlendStateCreateInfo.blendConstants[2] = 0.f;
	colorBlendStateCreateInfo.blendConstants[3] = 0.f;

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo =
		VulkanInitalizers::vkGraphicsPipelineCreateInfo(
			pipelineLayout,
			mTempRenderPass
		);

	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.stageCount = 2;
	graphicsPipelineCreateInfo.pStages = &shaderStages[0];
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &multisamplingStateCreateInfo;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;

	// todo: will leak
	if(vkCreateGraphicsPipelines(
		mApplication.logicalDevice,
		VK_NULL_HANDLE,
		1,
		&graphicsPipelineCreateInfo,
		nullptr,
		&mTempGraphicsPipeline
	) != VK_SUCCESS)
	{
		assert(false);
	}
}

void VulkanDriver::CreateFramebuffers()
{
	// Create a framebuffer for each swap chain image
	const auto& swapchainImagesViews = mApplication.swapchainImageViews;

	mSwapchainFramebuffers.resize(swapchainImagesViews.size());
	for(uint32_t i = 0; i < mApplication.swapchainImageViews.size(); i++)
	{
		const auto& imageView = swapchainImagesViews[i];
		mSwapchainFramebuffers[i] = CreateFramebuffer(imageView);
	}
}

VkFramebuffer VulkanDriver::CreateFramebuffer(VkImageView imageView)
{
	VkFramebufferCreateInfo framebufferCreateInfo =
		VulkanInitalizers::vkFramebufferCreateInfo(
			mTempRenderPass,
			1,
			&imageView,
			GetDriverSettings().renderWidth,
			GetDriverSettings().renderHeight
		);
	
	VkFramebuffer framebuffer;
	if(vkCreateFramebuffer(
			mApplication.logicalDevice,
			&framebufferCreateInfo,
			nullptr,
			&framebuffer
		) != VK_SUCCESS)
	{
		assert(false);
	}

	return(framebuffer);
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

void VulkanDriver::PrepareFrame(VkCommandBuffer commandBuffer, VkImage image, VkImageView imageView, VkFramebuffer framebuffer)
{
	VkCommandBufferBeginInfo commandBufferBeginInfo = 
		VulkanInitalizers::vkCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

	VkImageSubresourceRange imageSubresourceRange = 
		VulkanInitalizers::vkImageSubresourceRange(
			VK_IMAGE_ASPECT_COLOR_BIT
		);

	VkImageMemoryBarrier presentDrawBarrier =
		VulkanInitalizers::vkImageMemoryBarrier(
			VK_ACCESS_MEMORY_READ_BIT,
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			mApplication.queueIndices.graphicsFamily,
			mApplication.queueIndices.presentFamily,
			image,
			imageSubresourceRange
		);

	vkCmdPipelineBarrier(
		commandBuffer,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		NO_FLAGS,
		0,
		nullptr,
		0,
		nullptr,
		1,
		&presentDrawBarrier
	);

	if(mApplication.presentQueue != mApplication.graphicsQueue)
	{
		VkImageMemoryBarrier presentBarrier =
		VulkanInitalizers::vkImageMemoryBarrier(
			VK_ACCESS_MEMORY_READ_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			mApplication.queueIndices.presentFamily,
			mApplication.queueIndices.graphicsFamily,
			image,
			imageSubresourceRange
		);

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			NO_FLAGS,
			0,
			nullptr,
			0, nullptr,
			1,
			&presentBarrier
		);
	}

	// Clear the scren
	VkClearValue clearValue = {
		{1.f, 0.f, 0.f, 1.0f },
	};

	VkRenderPassBeginInfo renderPassBeginInfo = VulkanInitalizers::vkRenderPassBeginInfo(
		mTempRenderPass,
		framebuffer,
		VulkanInitalizers::vkRect2DScissor(mDriverSettings.renderWidth, mDriverSettings.renderHeight),
		1,
		&clearValue
	);

	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mTempGraphicsPipeline);

	VkViewport viewport = VulkanInitalizers::vkViewport(
		GetDriverSettings().renderWidth,
		GetDriverSettings().renderHeight
	);

	VkRect2D viewportScissor = VulkanInitalizers::vkRect2DScissor(
		GetDriverSettings().renderWidth,
		GetDriverSettings().renderHeight
	);

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &viewportScissor);

	VkDeviceSize offset = 0;

	//vkCmdBindVertexBuffers(commandBuffer, 0, 1, todo: handle, &offset );
	//vkCmdDraw(commandBuffer, 4, 1, 0, 0 );

	vkCmdEndRenderPass(commandBuffer);

	if(mApplication.presentQueue != mApplication.graphicsQueue)
	{
		VkImageMemoryBarrier drawPresentbarrier = VulkanInitalizers::vkImageMemoryBarrier(
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			mApplication.queueIndices.presentFamily,
			mApplication.queueIndices.graphicsFamily,
			image,
			imageSubresourceRange
		);

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			0,
			0,
			nullptr,
			0, nullptr,
			1,
			&drawPresentbarrier
		);
	}
}

void VulkanDriver::DrawFrame()
{
	PrepareFrame();

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

	VkPresentInfoKHR presentInfo;
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

void VulkanDriver::RenderGeometry()
{
}

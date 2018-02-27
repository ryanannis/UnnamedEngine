#include "VulkanDriver.h"

#include <memory>

#include "Engine/Graphics/VulkanDriver/VulkanCommon.h"

#include "Engine/Base/Utility/SingletonLogger.h"
#include "Engine/Graphics/VulkanDriver/VulkanApplicationFactory.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/ShaderResource.h"
#include "Engine/Graphics/VulkanDriver/VulkanUtils.h"
#include "Engine/Graphics/VulkanDriver/VulkanInitializers.h"
#include "Engine/Graphics/VulkanDriver/VulkanEngineResources.h"

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
	mApplication.shaderManager = std::make_unique<VulkanShaderManager>(mResourceManager, &mApplication);
	mApplication.meshManager = std::make_unique<VulkanMeshManager>(mResourceManager, &mApplication);
	
	VulkanUtils::Mesh::SubmeshBindingDescription bindingDescription;
	bindingDescription.vertexBinding = {};
	
	CreateRenderPass();
	CreatePipeline(bindingDescription);
}

void VulkanDriver::CreateRenderPass()
{
	VkAttachmentDescription attachmentDescription = {};
	attachmentDescription.flags = 0;
	attachmentDescription.format = mApplication.defaultSwapchainFormat;
	attachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
	attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachments = {};
	colorAttachments.attachment = 0;
	colorAttachments.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = nullptr;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachments;
	subpass.pResolveAttachments = nullptr;
	subpass.pDepthStencilAttachment = nullptr;
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments = nullptr;

	VkSubpassDependency preRenderPassColorAttachmentDependency =
		VulkanInitalizers::vkSubpassDependency(
			VK_SUBPASS_EXTERNAL,
			0,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			VK_DEPENDENCY_BY_REGION_BIT);

	VkSubpassDependency postRenderPassColorAttachmentDependency =
		VulkanInitalizers::vkSubpassDependency(
			0,
			VK_SUBPASS_EXTERNAL,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_DEPENDENCY_BY_REGION_BIT
		);

	const uint32_t numSubpassDependencies = 2;
	VkSubpassDependency subpassDependencies[2] = {
		preRenderPassColorAttachmentDependency,
		postRenderPassColorAttachmentDependency
	};

	VkRenderPassCreateInfo renderPassCreateInfo = {};
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

void VulkanDriver::RenderMesh(ResourceType<ModelResource> m)
{
	mApplication.meshManager->CreateMesh(m);

	if(mApplication.meshManager->RequiresFlush())
	{
		VkCommandBuffer commandBuffer;
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = VulkanInitalizers::vkCommandBufferAllocateInfo(
			mApplication.GetGraphicsCommandPool(),
			VK_COMMAND_BUFFER_LEVEL_PRIMARY
		);
		vkAllocateCommandBuffers(mApplication.logicalDevice, &commandBufferAllocateInfo, &commandBuffer);

		VkCommandBufferBeginInfo commandBufferBeginInfo =
			VulkanInitalizers::vkCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);
		mApplication.meshManager->FlushLoadQueue(commandBuffer);
		vkEndCommandBuffer(commandBuffer);

		vkQueueSubmit(mApplication.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(mApplication.graphicsQueue);
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

void VulkanDriver::CreatePipeline(VulkanUtils::Mesh::SubmeshBindingDescription bindingDescription)
{
	auto pipelineLayout = CreatePipelineLayout();

	ShaderHandle vertShader = mApplication.shaderManager->CreateShaderModule(VulkanEngineResources::basicVertexShader);
	ShaderHandle fragShader = mApplication.shaderManager->CreateShaderModule(VulkanEngineResources::basicFragmentShader);

	VkPipelineShaderStageCreateInfo shaderStages[2] = {
		mApplication.shaderManager->GetShaderPipelineInfo(vertShader),
		mApplication.shaderManager->GetShaderPipelineInfo(fragShader)
	};
	
	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo = 
		VulkanInitalizers::vkPipelineVertexInputStateCreateInfo();

	vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
	vertexInputStateCreateInfo.pVertexBindingDescriptions = &bindingDescription.vertexBinding;
	vertexInputStateCreateInfo.vertexAttributeDescriptionCount = bindingDescription.attributeBindings.size();
	vertexInputStateCreateInfo.pVertexAttributeDescriptions = bindingDescription.attributeBindings.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo = 
		VulkanInitalizers::vkPipelineInputAssemblyStateCreateInfo(
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			VK_FALSE,
			0
		);

	VkViewport viewport = VulkanInitalizers::vkViewport(
		static_cast<float>(GetDriverSettings().renderWidth),
		static_cast<float>(GetDriverSettings().renderHeight)
	);
	
	VkRect2D viewportScissor = VulkanInitalizers::vkRect2DScissor(
		static_cast<uint32_t>(GetDriverSettings().renderWidth),
		static_cast<uint32_t>(GetDriverSettings().renderHeight)
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

	std::vector<VkDynamicState> dynamicStates = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_SCISSOR,
    };

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo = {};
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicStateCreateInfo.pDynamicStates = dynamicStates.data();

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
	graphicsPipelineCreateInfo.pDynamicState = &dynamicStateCreateInfo;

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
	for(SwapChainImageData data : mApplication.swapChainData) {
		vkDestroyImageView(mApplication.logicalDevice, data.imageView, nullptr);
	}
	vkDestroySurfaceKHR(mApplication.instance, mApplication.surface, nullptr);
	vkDestroySwapchainKHR(mApplication.logicalDevice, mApplication.defaultSwapchain, nullptr);
	vkDestroyDevice(mApplication.logicalDevice, nullptr);
	vkDestroyInstance(mApplication.instance, nullptr);
}

RenderData VulkanDriver::BuildRenderData(uint32_t swapChainIndex)
{
	VkCommandBuffer commandBuffer;
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = VulkanInitalizers::vkCommandBufferAllocateInfo(
		mApplication.GetGraphicsCommandPool(),
		VK_COMMAND_BUFFER_LEVEL_PRIMARY
	);
	vkAllocateCommandBuffers(mApplication.logicalDevice, &commandBufferAllocateInfo, &commandBuffer);

	VkFramebuffer framebuffer = CreateFramebuffer(mApplication.swapChainData[swapChainIndex].imageView);

	RenderData renderData = {};
	renderData.commandBuffer = commandBuffer;
	renderData.framebuffer = framebuffer;
	renderData.imageAvailableSemaphore = mApplication.imageAvailableSemaphore;
	renderData.finishedRenderingSemaphore = mApplication.finishedRenderingSemaphore;
	renderData.image = mApplication.swapChainData[swapChainIndex].image;

	return(renderData);
}

void VulkanDriver::PrepareFrameCommandBuffer(const RenderData& renderData)
{
	VkCommandBufferBeginInfo commandBufferBeginInfo = 
		VulkanInitalizers::vkCommandBufferBeginInfo(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	vkBeginCommandBuffer(renderData.commandBuffer, &commandBufferBeginInfo);

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
			renderData.image,
			imageSubresourceRange
		);

	vkCmdPipelineBarrier(
		renderData.commandBuffer,
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
			renderData.image,
			imageSubresourceRange
		);

		vkCmdPipelineBarrier(
			renderData.commandBuffer,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			NO_FLAGS,
			0,
			nullptr,
			0, 
			nullptr,
			1,
			&presentBarrier
		);
	}

	// Clear the scren
	VkClearValue clearValue = {{1.f, 0.f, 0.f, 1.0f}};

	VkRenderPassBeginInfo renderPassBeginInfo = VulkanInitalizers::vkRenderPassBeginInfo(
		mTempRenderPass,
		renderData.framebuffer,
		VulkanInitalizers::vkRect2DScissor(mDriverSettings.renderWidth, mDriverSettings.renderHeight),
		1,
		&clearValue
	);

	VkViewport viewport = VulkanInitalizers::vkViewport(
		static_cast<float>(GetDriverSettings().renderWidth),
		static_cast<float>(GetDriverSettings().renderHeight)
	);

	VkRect2D viewportScissor = VulkanInitalizers::vkRect2DScissor(
		GetDriverSettings().renderWidth,
		GetDriverSettings().renderHeight
	);

	vkCmdSetViewport(renderData.commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(renderData.commandBuffer, 0, 1, &viewportScissor);

	VkDeviceSize offset = 0;
	
	vkCmdBeginRenderPass(renderData.commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(renderData.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mTempGraphicsPipeline);

	RenderGeometry(renderData.commandBuffer);

	vkCmdEndRenderPass(renderData.commandBuffer);

	if(mApplication.presentQueue != mApplication.graphicsQueue)
	{
		VkImageMemoryBarrier drawPresentbarrier = VulkanInitalizers::vkImageMemoryBarrier(
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			mApplication.queueIndices.presentFamily,
			mApplication.queueIndices.graphicsFamily,
			renderData.image,
			imageSubresourceRange
		);

		vkCmdPipelineBarrier(
			renderData.commandBuffer,
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

	vkEndCommandBuffer(renderData.commandBuffer);
}

void VulkanDriver::DrawFrame()
{
	VkFenceCreateInfo fenceInfo = VulkanInitalizers::vkFenceCreateInfo();
	VkFence fence;
	vkCreateFence(mApplication.logicalDevice, &fenceInfo, nullptr, &fence);

	// Acquire image
	uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
		mApplication.logicalDevice, 
		mApplication.defaultSwapchain, 
		std::numeric_limits<uint64_t>::max(),
		mApplication.imageAvailableSemaphore, 
		fence, 
		&imageIndex
	);

	// Get resources for rendering
	RenderData renderData = BuildRenderData(imageIndex);


    if(result == VK_ERROR_OUT_OF_DATE_KHR){
		assert(false); // todo: allow resize without breaking
        return;
    }
	else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		assert(false);
    }

	// Prepare commands
	PrepareFrameCommandBuffer(renderData);

	VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
	VkSubmitInfo submitInfo = VulkanInitalizers::vkSubmitInfo();
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &mApplication.imageAvailableSemaphore;
	submitInfo.pWaitDstStageMask = &waitFlags;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &renderData.commandBuffer;
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &mApplication.finishedRenderingSemaphore;

	const VkResult presentQueueResult = vkQueueSubmit(mApplication.presentQueue, 1, &submitInfo, VK_NULL_HANDLE);
	if(presentQueueResult != VK_SUCCESS)
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

	VkResult presentRes = vkQueuePresentKHR(mApplication.presentQueue, &presentInfo);
	if(presentRes != VK_SUCCESS)
	{
		assert(false);
	}

	vkWaitForFences(
		mApplication.logicalDevice,
		1,
		&fence,
		VK_TRUE,
		DEFAULT_FENCE_TIMEOUT
	);
}

void VulkanDriver::RenderGeometry(VkCommandBuffer commandBuffer)
{
	std::vector<SubmeshAllocation> meshes = mApplication.meshManager->GetMeshesWithLayout(UE_MESHLAYOUT_NORMALS_1UV);
	for(size_t i = 0; i < meshes.size(); i++)
	{
		const SubmeshAllocation allocation = meshes[i];
		const VkDeviceSize verticesOffset = allocation.GetVerticesOffset();
		const VkDeviceSize indicesOffset = allocation.GetIndicesOffset();

		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &allocation.buffer, &verticesOffset);
		vkCmdBindIndexBuffer(commandBuffer, allocation.buffer, indicesOffset, VK_INDEX_TYPE_UINT32);
		
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(allocation.numIndices), 1, 0, 0, 0);
	}
}

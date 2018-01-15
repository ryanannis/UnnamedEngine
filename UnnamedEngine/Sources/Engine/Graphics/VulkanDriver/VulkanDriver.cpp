#include "VulkanDriver.h"
#include "Engine/Base/Utility/SingletonLogger.h"
#include "Engine/Graphics/VulkanDriver/VulkanApplicationFactory.h"
#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Base/Resource/ShaderResource.h"
#include "Engine/Graphics/VulkanDriver/VulkanUtils.h"

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
	VkPipelineLayoutCreateInfo layoutCreateInfo;
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
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

	const static std::string basicVertexShader("Engine/Basic.vert");
	const static std::string basicFragmentShader("Engine/Basic.frag");
	SubmeshData* submesh = nullptr;

	VkShaderModule vertShader = CreateShaderModule(basicVertexShader);
	VkShaderModule fragShader = CreateShaderModule(basicFragmentShader);

	VkPipelineShaderStageCreateInfo vertStage;
	vertStage.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vertStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertStage.module = vertShader;
	vertStage.pName = "main";
	vertStage.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo fragStage;
	vertStage.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	vertStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vertStage.module = fragShader;
	vertStage.pName = "main";
	vertStage.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo shaderStages[2] = {
		vertStage,
		fragStage
	};

	const auto submeshAttributes = VulkanUtils::Mesh::ComputeSubmeshBindingDescription(submesh);
	
	VkPipelineVertexInputStateCreateInfo vertexInputStateCreateInfo;
	vertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputStateCreateInfo.vertexBindingDescriptionCount = 1;
	vertexInputStateCreateInfo.pVertexBindingDescriptions = &submeshAttributes.vertexBinding;
	vertexInputStateCreateInfo.vertexAttributeDescriptionCount = submeshAttributes.attributeBindings.size();
	vertexInputStateCreateInfo.pVertexAttributeDescriptions = &submeshAttributes.attributeBindings[0];

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
	inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport;
	viewport.x = 0.f;
	viewport.y = 0.f;
	viewport.width = GetDriverSettings().renderWidth;
	viewport.height = GetDriverSettings().renderHeight;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.0f;

	VkRect2D viewportScissor;
	viewportScissor.offset = VkOffset2D{0,0};
	viewportScissor.extent = VkExtent2D{
		GetDriverSettings().renderWidth, 
		GetDriverSettings().renderHeight
	};

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo;
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &viewportScissor;

	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
	rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	rasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
	rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizationStateCreateInfo.depthBiasConstantFactor = 0.f;
	rasterizationStateCreateInfo.depthBiasClamp = 0.f;
	rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.f;
	rasterizationStateCreateInfo.lineWidth = 1.f;

	VkPipelineMultisampleStateCreateInfo multisamplingStateCreateInfo;
	multisamplingStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisamplingStateCreateInfo.sampleShadingEnable = VK_FALSE;
	multisamplingStateCreateInfo.minSampleShading = 1.f;
	multisamplingStateCreateInfo.pSampleMask = nullptr;
	multisamplingStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
	multisamplingStateCreateInfo.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState;
	colorBlendAttachmentState.blendEnable = VK_FALSE;
	colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo;
	colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
	colorBlendStateCreateInfo.logicOp = VK_LOGIC_OP_COPY;
	colorBlendStateCreateInfo.attachmentCount = 1;
	colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
	colorBlendStateCreateInfo.blendConstants[0] = 0.f;
	colorBlendStateCreateInfo.blendConstants[1] = 0.f;
	colorBlendStateCreateInfo.blendConstants[2] = 0.f;
	colorBlendStateCreateInfo.blendConstants[3] = 0.f;

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo;
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.flags = 0;
	

	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo;
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.stageCount = 2;
	graphicsPipelineCreateInfo.pStages = &shaderStages[0];
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputStateCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.pTessellationState = nullptr;
	graphicsPipelineCreateInfo.pViewportState = &viewportStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &multisamplingStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendStateCreateInfo;
	graphicsPipelineCreateInfo.pDynamicState = nullptr;
	graphicsPipelineCreateInfo.layout = pipelineLayout;
	graphicsPipelineCreateInfo.renderPass = mTempRenderPass;
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = 0;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;

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

VkShaderModule VulkanDriver::CreateShaderModule(std::string shader)
{
	const auto bytecode = mResourceManager->LoadResource(ResourceType<ShaderResource>(shader))->GetShaderBinary();
	
	VkShaderModule module;

	VkShaderModuleCreateInfo shaderModule;
	shaderModule.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModule.codeSize = bytecode.size();
	shaderModule.pCode = reinterpret_cast<const uint32_t*>(bytecode.data());

	if(vkCreateShaderModule(
		mApplication.logicalDevice,
		&shaderModule,
		nullptr,
		&module
	) != VK_SUCCESS)
	{
		assert(false);
	}

	return(module);
}

void VulkanDriver::CreateFramebuffers()
{
	// Create a framebuffer for each swap hcain image
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
	VkFramebufferCreateInfo framebufferCreateInfo;
	framebufferCreateInfo.renderPass = mTempRenderPass;
	framebufferCreateInfo.attachmentCount = 1;
	framebufferCreateInfo.pAttachments = &imageView;
	framebufferCreateInfo.width = GetDriverSettings().renderWidth;
	framebufferCreateInfo.height = GetDriverSettings().renderHeight;
	framebufferCreateInfo.layers = 1;
	
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
	VkCommandBufferBeginInfo commandBufferBeginInfo;
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	commandBufferBeginInfo.pInheritanceInfo = nullptr;

	//------------------ BEGIN COMMAND BUFFER RECORDING -----------------------------------//

	vkBeginCommandBuffer(commandBuffer, &commandBufferBeginInfo);

	VkImageSubresourceRange imageSubresourceRange;
	imageSubresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageSubresourceRange.baseMipLevel = 0;
	imageSubresourceRange.levelCount = 1;
	imageSubresourceRange.baseArrayLayer = 0;
	imageSubresourceRange.layerCount = 1;

	VkImageMemoryBarrier presentDrawbarrier;
	presentDrawbarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	presentDrawbarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	presentDrawbarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	presentDrawbarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	presentDrawbarrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	presentDrawbarrier.srcQueueFamilyIndex = mApplication.queueIndices.graphicsFamily;
	presentDrawbarrier.dstQueueFamilyIndex = mApplication.queueIndices.presentFamily;
	presentDrawbarrier.image = image;
	presentDrawbarrier.subresourceRange = imageSubresourceRange;

	vkCmdPipelineBarrier(
		commandBuffer,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		0,
		0,
		nullptr,
		0,
		nullptr,
		1,
		&presentDrawbarrier
	);

	if(mApplication.presentQueue != mApplication.graphicsQueue)
	{
		VkImageMemoryBarrier presentBarrier;
		presentBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		presentBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		presentBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		presentBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		presentBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		presentBarrier.srcQueueFamilyIndex = mApplication.queueIndices.presentFamily;
		presentBarrier.dstQueueFamilyIndex = mApplication.queueIndices.graphicsFamily;
		presentBarrier.image = image;
		presentBarrier.subresourceRange = imageSubresourceRange;

		vkCmdPipelineBarrier(
			commandBuffer,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			0,
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

	VkRenderPassBeginInfo renderPassBeginInfo;
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.renderPass = mTempRenderPass;
	renderPassBeginInfo.framebuffer = framebuffer;
	renderPassBeginInfo.renderArea = VkRect2D{
		VkOffset2D{0,0},
		VkExtent2D{mDriverSettings.renderWidth, mDriverSettings.renderHeight}
	};
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearValue;

	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mTempGraphicsPipeline);

	VkViewport viewport;
	viewport.x = 0.f;
	viewport.y = 0.f;
	viewport.width = GetDriverSettings().renderWidth;
	viewport.height = GetDriverSettings().renderHeight;
	viewport.minDepth = 0.f;
	viewport.maxDepth = 1.0f;

	VkRect2D viewportScissor;
	viewportScissor.offset = VkOffset2D{0,0};
	viewportScissor.extent = VkExtent2D{
		GetDriverSettings().renderWidth, 
		GetDriverSettings().renderHeight
	};

	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
	vkCmdSetScissor(commandBuffer, 0, 1, &viewportScissor);

	VkDeviceSize offset = 0;
	//vkCmdBindVertexBuffers(commandBuffer, 0, 1, todo: handle, &offset );
	//vkCmdDraw(commandBuffer, 4, 1, 0, 0 );
	vkCmdEndRenderPass(commandBuffer);

	if(mApplication.presentQueue != mApplication.graphicsQueue)
	{
		VkImageMemoryBarrier drawPresentbarrier;
		drawPresentbarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		drawPresentbarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		drawPresentbarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		drawPresentbarrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		drawPresentbarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		drawPresentbarrier.srcQueueFamilyIndex = mApplication.queueIndices.presentFamily;
		drawPresentbarrier.dstQueueFamilyIndex = mApplication.queueIndices.graphicsFamily;
		drawPresentbarrier.image = image;
		drawPresentbarrier.subresourceRange = imageSubresourceRange;

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

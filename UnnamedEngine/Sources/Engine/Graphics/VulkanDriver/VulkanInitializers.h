#pragma once

#include <vulkan/vulkan.h>

#define NO_FLAGS 0

/* 
 * This namespace provides initializers for Vulkan structs.
 * There are initializers provided that correctly initialize sType and any additional required variables for
 * most structs.
 *
 * Additionaly, there are convenience intializers for some common patterns designed to reduce boilerplate and
 * code noise.
 */
namespace VulkanInitalizers
{
	inline VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo()
	{
		VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo = {};

		vkPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		return(vkPipelineLayoutCreateInfo);
	}

	inline VkViewport vkViewport(float width, float height)
	{
		VkViewport viewport = {};

		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = width;
		viewport.height = height;
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		return(viewport);
	}

	inline VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo(
		VkPolygonMode polygonMode,
		VkCullModeFlags cullMode,
		VkFrontFace frontFace
	)
	{
		VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = {};

		pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		pipelineRasterizationStateCreateInfo.polygonMode = polygonMode;
		pipelineRasterizationStateCreateInfo.cullMode = cullMode;
		pipelineRasterizationStateCreateInfo.frontFace = frontFace;

		return(pipelineRasterizationStateCreateInfo);
	}

	inline VkPipelineViewportStateCreateInfo vkPipelineViewportStateCreateInfo(
		uint32_t viewportCount
	)
	{
		VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo = {};

		pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		pipelineViewportStateCreateInfo.viewportCount = viewportCount;
		pipelineViewportStateCreateInfo.scissorCount = viewportCount;

		return(pipelineViewportStateCreateInfo);
	}

	inline VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo(
		VkSampleCountFlagBits rasterizationSamples
	)
	{
		VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo = {};

		pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		pipelineMultisampleStateCreateInfo.rasterizationSamples = rasterizationSamples;

		return(pipelineMultisampleStateCreateInfo);
	}

	inline VkPipelineColorBlendAttachmentState vkPipelineColorBlendAttachmentState(
		VkBool32 blendEnable,
		VkColorComponentFlags colorWriteMask
	)
	{
		VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = {};

		pipelineColorBlendAttachmentState.blendEnable = blendEnable;
		pipelineColorBlendAttachmentState.colorWriteMask = colorWriteMask;

		return(pipelineColorBlendAttachmentState);
	}

	inline VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo(
		VkPrimitiveTopology primitiveTopology,
		VkBool32 primitiveRestartEnable,
		VkPipelineInputAssemblyStateCreateFlags flags = 0
	)
	{
		VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo = {};

		pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
		pipelineInputAssemblyStateCreateInfo.flags = flags;

		return(pipelineInputAssemblyStateCreateInfo);
	}

	inline VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo(
		VkPipelineLayout layout,
		VkRenderPass renderPass,
		VkPipelineCreateFlags flags = 0
	)
	{
		VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};

		graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		graphicsPipelineCreateInfo.layout = layout;
		graphicsPipelineCreateInfo.renderPass = renderPass;
		graphicsPipelineCreateInfo.flags = flags;
		graphicsPipelineCreateInfo.basePipelineIndex = -1;
		graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

		return(graphicsPipelineCreateInfo);
	}

	inline VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateCreateInfo()
	{
		VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo = {};

		pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

		return(pipelineColorBlendStateCreateInfo);
	}

	inline VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo()
	{
		VkPipelineVertexInputStateCreateInfo pipelineVertexInputStateCreateInfo = {};

		pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		return(pipelineVertexInputStateCreateInfo);
	}

	inline VkCommandBufferBeginInfo vkCommandBufferBeginInfo(
		VkCommandBufferUsageFlags flags = 0
	)
	{
		VkCommandBufferBeginInfo commandBufferBeginInfo = {};

		commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		commandBufferBeginInfo.flags = flags;

		return(commandBufferBeginInfo);
	}

	inline VkBufferCreateInfo vkBufferCreateInfo()
	{
		VkBufferCreateInfo bufferCreateInfo = {};

		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

		return(bufferCreateInfo);
	}

	inline VkImageMemoryBarrier vkImageMemoryBarrier(
		VkAccessFlags srcAccessMask,
		VkAccessFlags dstAccessMask,
		VkImageLayout oldLayout,
		VkImageLayout newLayout,
		uint32_t srcQueueFamilyIndex,
		uint32_t dstQueueFamilyIndex,
		VkImage	image,
		VkImageSubresourceRange subresourceRange
	)
	{
		VkImageMemoryBarrier imageMemoryBarrier = {};

		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcAccessMask = srcAccessMask;
		imageMemoryBarrier.dstAccessMask = dstAccessMask;
		imageMemoryBarrier.oldLayout = oldLayout;
		imageMemoryBarrier.newLayout = newLayout;
		imageMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
		imageMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;


		return(imageMemoryBarrier);
	}

	inline VkImageSubresourceRange vkImageSubresourceRange(
		VkImageAspectFlags    aspectMask,
		uint32_t              baseMipLevel = 0,
		uint32_t              levelCount = 1,
		uint32_t              baseArrayLayer = 0,
		uint32_t              layerCount = 1
	)
	{
		VkImageSubresourceRange imageSubresourceRange = {};

		imageSubresourceRange.baseMipLevel = baseMipLevel;
		imageSubresourceRange.levelCount = levelCount;
		imageSubresourceRange.baseArrayLayer = baseArrayLayer;
		imageSubresourceRange.layerCount = layerCount;

		return(imageSubresourceRange);
	}

	inline VkRenderPassBeginInfo vkRenderPassBeginInfo(
		VkRenderPass renderPass,
		VkFramebuffer framebuffer,
		VkRect2D renderArea,
		uint32_t clearValueCount,
		const VkClearValue* pClearValues
	)
	{
		VkRenderPassBeginInfo renderPassBeginInfo = {};

		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass;
		renderPassBeginInfo.framebuffer;
		renderPassBeginInfo.renderArea;
		renderPassBeginInfo.clearValueCount;
		renderPassBeginInfo.pClearValues;

		return(renderPassBeginInfo);
	}

	inline VkRect2D vkRect2DScissor(float width, float height)
	{
		VkRect2D rect2D = {};
		
		rect2D.extent = { width, height };

		return(rect2D);
	}

	inline VkFramebufferCreateInfo vkFramebufferCreateInfo(
		VkRenderPass renderPass,
		uint32_t attachmentCount,
		const VkImageView* pAttachments,
		uint32_t width,
		uint32_t height,
		uint32_t layers = 1
	)
	{
		VkFramebufferCreateInfo framebufferCreateinfo = {};
		
		assert(renderPass);
		assert(attachmentCount == 0 || pAttachments != nullptr);

		framebufferCreateinfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateinfo.renderPass = renderPass;
		framebufferCreateinfo.attachmentCount = attachmentCount;
		framebufferCreateinfo.pAttachments = pAttachments;
		framebufferCreateinfo.width = width;
		framebufferCreateinfo.height = height;
		framebufferCreateinfo.layers = layers;

		return(framebufferCreateinfo);
	}

	inline VkSubmitInfo vkSubmitInfo(
		uint32_t commandBufferCount,
		const VkCommandBuffer* pCommandBuffers
	)
	{
		VkSubmitInfo submitInfo = {};

		submitInfo.commandBufferCount = commandBufferCount;
		submitInfo.pCommandBuffers = pCommandBuffers;
	}

	inline VkFenceCreateInfo vkFenceCreateInfo(VkFenceCreateFlags flags = 0)
	{
		VkFenceCreateInfo fenceCreateInfo = {};

		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = flags;

		return(fenceCreateInfo);
	}

	inline VkCommandPoolCreateInfo vkCommandPoolCreateInfo(
		VkCommandPoolCreateFlags flags,
		uint32_t queueFamilyIndex
	)
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo = {};

		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.flags = flags;
		commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

		return(commandPoolCreateInfo);
	}

	inline VkCommandBufferAllocateInfo vkCommandBufferAllocateInfo(
		VkCommandPool commandPool,
		VkCommandBufferLevel level,
		uint32_t commandBufferCount = 1
	)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};

		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.commandBufferCount = commandBufferCount;

		return(commandBufferAllocateInfo);
	}
}
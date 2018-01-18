#pragma once

#include <vulkan/vulkan.h>

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
}
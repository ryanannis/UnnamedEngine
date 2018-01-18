#pragma once

#include <vulkan/vulkan.h>

namespace VulkanInitalizers
{
	inline VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo()
	{
		VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo = {};
		vkPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	}
}
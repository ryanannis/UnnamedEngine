#pragma once
#include "Engine/Base/Common/Common.h"
#include <vulkan\vulkan.h>

#include "Engine/Graphics/VulkanDriver/VulkanInitializers.h"
#include "Engine/Graphics/VulkanDriver/VulkanApplication.h"
#include "Engine/Graphics/VulkanDriver/VulkanCommon.h"

namespace VulkanUtils
{
	namespace Device
	{
		/*
		* Flushes the command buffer onto the given queue and returns
		* when queue execution is finished.
		* BEWARE: USES A FENCE. DO NOT USE UNLESS NECESSARY (EXPENSIVE).
		*/
		inline void FlushCommandBuffer(VulkanApplication* application, VkCommandBuffer commandBuffer, VkQueue queue, bool freeCommandBuffer = true   )
		{
			assert(commandBuffer != VK_NULL_HANDLE);

			VkSubmitInfo submitInfo = VulkanInitalizers::vkSubmitInfo(
				1,
				&commandBuffer
			);

			VkFenceCreateInfo fenceInfo = VulkanInitalizers::vkFenceCreateInfo();
			
			VkFence fence;
			vkCreateFence(application->logicalDevice, &fenceInfo, nullptr, &fence);
			vkQueueSubmit(queue, 1, &submitInfo, fence);
			vkWaitForFences(application->logicalDevice, 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT);

			if(freeCommandBuffer)
			{
				vkFreeCommandBuffers(application->logicalDevice, application->GetGraphicsCommandPool(), 1, &commandBuffer);
			}
		}
	}
}
#include "VulkanDriver.h"
#include "Engine/Base/Utility/SingletonLogger.h"

#include "Engine/Graphics/VulkanDriver/VulkanApplicationFactory.h"

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

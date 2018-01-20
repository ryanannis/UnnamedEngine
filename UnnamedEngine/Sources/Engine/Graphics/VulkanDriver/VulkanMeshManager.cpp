#include "VulkanMeshManager.h"

#include "vk_mem_alloc.h"

#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Graphics/VulkanDriver/VulkanInitializers.h"
#include "Engine/Graphics/VulkanDriver/VulkanApplication.h"

VulkanMeshManager::VulkanMeshManager(Ptr<ResourceManager> resManager, VulkanApplication* application) :
	mResourceManager(resManager),
	mApplication(application)
{}

MeshHandle VulkanMeshManager::CreateMesh(URI resourceLocation)
{
	return(CreateMesh(ResourceType<ModelResource>(resourceLocation)));
}

MeshHandle VulkanMeshManager::CreateMesh(ResourceType<ModelResource> res)
{
	// Early out if we already have mesh on device.
	auto existingResource = mHandleMap.find(res.GetURI().GetHash());
	if(existingResource != mHandleMap.end())
	{
		return(existingResource->second);
	}

	auto meshResource = mResourceManager->LoadResource(res);
	if(!meshResource)
	{
		// Failed to load mesh!
		assert(false);
	}
	
	auto nextHandle = GetFreeHandle();
	mHandleMap.insert(std::make_pair(res.GetURI().GetHash(), nextHandle));

	MeshInfo& meshInfo = mModels[nextHandle];
	meshInfo.handle = NULL_MESH_HANDLE;
	meshInfo.submeshAllocations = std::vector<SubmeshAllocation>();

	return(nextHandle);

	// Let the mesh fall out of scope from the regular resource manager.
	// todo:  This is technically a waste of a smart ptr copy and alloc
}

MeshInfo VulkanMeshManager::GetMeshInfo(MeshHandle h)
{
	assert(h < mModels.size());
	return(mModels[h]);
}

void VulkanMeshManager::FlushLoadQueue(VkCommandBuffer commandBuffer)
{
	while(!mLoadQueue.empty())
	{
		// todo:  could we do this deffered and make a bigass buffer for all ready meshes
		const QueuedMeshLoad& meshLoad = mLoadQueue.top();
		
		assert(meshLoad.model->IsReady());

		MeshData modelData = meshLoad.model->GetMeshes();

		std::vector<SubmeshAllocation> submeshAllocations;
		for(uint32_t i = 0; i < modelData.numSubmeshes; i++)
		{
			SubmeshAllocation submeshAllocation = {};

			const SubmeshData& submesh = modelData.submeshes[i];
			const uint32_t bufferSize = submesh.GetTotalBufferSize();

			// For static models without animation we use device-local memory
			if(meshLoad.model->IsStaticMesh())
			{
				// Create CPU-visible staging buffer
				VkBuffer stagingBuffer;
				VmaAllocation stagingAllocation;
				VkBufferCreateInfo bufferInfo = VulkanInitalizers::vkBufferCreateInfo();
				bufferInfo.size = bufferSize;
				bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

				VmaAllocationCreateInfo stagingAllocInfo = {};
				stagingAllocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

				vmaCreateBuffer(mApplication->allocator, &bufferInfo, &stagingAllocInfo, &stagingBuffer, &stagingAllocation, nullptr);

				// Copy mesh data to CPU-mapped staging buffer
				void* cpuMappedData;
				vmaMapMemory(mApplication->allocator, stagingAllocation, &cpuMappedData);
				memcpy(cpuMappedData, submesh.interleavedData,  bufferSize);
				vmaUnmapMemory(mApplication->allocator, stagingAllocation);

				/* Create GPU-local buffer*/
				VkBufferCreateInfo stagingBufferInfo = VulkanInitalizers::vkBufferCreateInfo();
				stagingBufferInfo.size = bufferSize;
				stagingBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

				VmaAllocationCreateInfo gpuAllocInfo = {};
				gpuAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

				VkBuffer destinationBuffer;
				VmaAllocation destinationAllocation;
				vmaCreateBuffer(mApplication->allocator, &bufferInfo, &gpuAllocInfo, &destinationBuffer, &destinationAllocation, nullptr);
				
				// todo: put in a fence to delete the staging buffers after the copy is performed, this currently LEAKS
				SubmeshAllocation s;
				submeshAllocations.push_back(s);

				VkBufferCopy copyRegion = {};
				copyRegion.size = bufferSize;

				vkCmdCopyBuffer(
					commandBuffer,
					stagingBuffer,
					destinationBuffer,
					1,
					&copyRegion
				);

				submeshAllocation.buffer = destinationBuffer;
				submeshAllocation.allocation = destinationAllocation;
			}
		}

		mLoadQueue.pop();
	}
}

MeshHandle VulkanMeshManager::GetFreeHandle()
{
	if(mFreedHandles.size() != 0)
	{
		auto nextHandle = mFreedHandles.top();
		mFreedHandles.pop();
		return(nextHandle);
	}

	MeshHandle nextNewHandle = mModels.size();
	mModels.resize(nextNewHandle + 1);

	return(nextNewHandle);
}

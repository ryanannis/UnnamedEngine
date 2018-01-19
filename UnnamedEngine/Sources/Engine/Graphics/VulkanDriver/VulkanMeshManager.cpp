#include "VulkanMeshManager.h"

#include "vk_mem_alloc.h"

#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Graphics/VulkanDriver/VulkanInitializers.h"

VulkanMeshManager::VulkanMeshManager(Ptr<ResourceManager> resManager, VulkanApplication* application) :
	mResourceManager(resManager),
	mApplication(application)
{}

MeshHandle VulkanMeshManager::CreateMesh(URI resourceLocation)
{
	return(CreateMesh(resourceLocation));
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
	
	const MeshData& modelData = meshResource->GetMeshes();

	auto nextHandle = GetFreeHandle();
	mHandleMap.insert(std::make_pair(res.GetURI().GetHash(), nextHandle));

	// Allocate buffers for submeshes

	std::vector<SubmeshAllocation> submeshAllocations;
	for(uint32_t i = 0; i < modelData.numSubmeshes; i++)
	{
		const SubmeshData& submesh = modelData.submeshes[i];
		const uint32_t bufferSize = submesh.GetTotalBufferSize();

		// Use simple heap allocation via AMD VMA for Buffers
		VkBufferCreateInfo bufferInfo = VulkanInitalizers::vkBufferCreateInfo();
		bufferInfo.size = bufferSize;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

		VmaAllocationCreateInfo allocInfo = {};
		allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

		VkBuffer buffer;
		VmaAllocation allocation;
		vmaCreateBuffer(mApplication->allocator, &bufferInfo, &allocInfo, &buffer, &allocation, nullptr);

		SubmeshAllocation s;
		submeshAllocations.push_back(s);
	}

	MeshInfo& meshInfo = mModels[nextHandle];
	meshInfo.submeshAllocations = submeshAllocations;

	return(nextHandle);

	// Let the mesh fall out of scope from the regular resource manager.
	// todo:  This is technically a waste of a smart ptr copy and alloc
}

MeshInfo VulkanMeshManager::GetMeshInfo(MeshHandle h)
{
	assert(h < mModels.size());
	return(mModels[h]);
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

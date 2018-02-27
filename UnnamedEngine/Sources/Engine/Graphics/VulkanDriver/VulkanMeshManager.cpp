#include "VulkanMeshManager.h"

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

	mLoadQueue.push(QueuedMeshLoad{ meshInfo.handle, meshResource });

	return(nextHandle);

	// Let the mesh fall out of scope from the regular resource manager.
	// todo:  This is technically a waste of a smart ptr copy and alloc
}

MeshInfo VulkanMeshManager::GetMeshInfo(MeshHandle h)
{
	assert(h < mModels.size());
	return(mModels[h]);
}

std::vector<SubmeshAllocation> VulkanMeshManager::GetMeshesWithLayout(const MeshLayout& meshLayout)
{
	std::vector<SubmeshAllocation> submeshes;

	// todo:  this may be able to be improved by chucking submeshes in seperate arrays and
	// sorting by format
	for(const MeshInfo& model : mModels)
	{
		for(const SubmeshAllocation& submesh : model.submeshAllocations)
		{
			if(meshLayout.numUVs == submesh.numUVs && submesh.meshFlags == meshLayout.properties)
			{
				submeshes.push_back(submesh);
			}
		}
	}

	return(submeshes);
}

bool VulkanMeshManager::RequiresFlush()
{
	return(!mLoadQueue.empty());
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
			const uint32_t bufferSize = submesh.GetVerticeBufferSize() + submesh.GetIndicesBufferSize();
		
			const uint32_t verticesBufferSize = submesh.GetVerticeBufferSize();
			const uint32_t indicesBufferSize = submesh.GetIndicesBufferSize();

			const uint32_t verticesBufferOffset = 0;
			const uint32_t indicesBufferOffset = submesh.GetVerticeBufferSize();

			// For static models without animation we use device-local memory
			if(meshLoad.model->IsStaticMesh())
			{
				// Create CPU-visible staging buffer
				VkBuffer stagingBuffer;
				VmaAllocation stagingAllocation;

				VkBufferCreateInfo staginBufferInfo = VulkanInitalizers::vkBufferCreateInfo();
				staginBufferInfo.size = bufferSize;
				staginBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

				VmaAllocationCreateInfo stagingAllocInfo = {};
				stagingAllocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;

				const VkResult staging_result = vmaCreateBuffer(mApplication->allocator, &staginBufferInfo, &stagingAllocInfo, &stagingBuffer, &stagingAllocation, nullptr);
				if(staging_result != VK_SUCCESS)
				{
					assert(false);
				}

				StagingBuffer stagingBufferStore{
					stagingBuffer,
					stagingAllocation
				};

				mStagingBuffers.push(stagingBufferStore);

				// Copy mesh data to host-mapped staging buffer
				void* hostMappedData = submesh.interleavedData;
				vmaMapMemory(mApplication->allocator, stagingAllocation, &hostMappedData);
				
				void* hostVerticesAddr = static_cast<void*>(static_cast<char*>(hostMappedData) + verticesBufferOffset);
				void* hostIndicesAddr = static_cast<void*>(static_cast<char*>(hostMappedData) + indicesBufferOffset);
				
				memcpy(hostVerticesAddr, submesh.interleavedData, verticesBufferSize);
				memcpy(hostIndicesAddr, submesh.interleavedData, indicesBufferSize);

				vmaUnmapMemory(mApplication->allocator, stagingAllocation);

				/* Create GPU-local buffer*/
				VkBufferCreateInfo resultBufferInfo = VulkanInitalizers::vkBufferCreateInfo();
				resultBufferInfo.size = bufferSize;
				resultBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

				VmaAllocationCreateInfo gpuAllocInfo = {};
				gpuAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

				VkBuffer destinationBuffer;
				VmaAllocation destinationAllocation;

				const VkResult gpuBufferResult = vmaCreateBuffer(mApplication->allocator, &resultBufferInfo, &gpuAllocInfo, &destinationBuffer, &destinationAllocation, nullptr);
				if(gpuBufferResult != VK_SUCCESS)
				{
					assert(false);
				}
				
				// todo: delete staging buffers after they finish copying
				SubmeshAllocation s;
				s.allocation = destinationAllocation;
				s.buffer = destinationBuffer;
				s.numUVs = submesh.numUVs;
				s.meshFlags = submesh.properties;
				s.verticesOffset = verticesBufferOffset;
				s.indicesOffset = indicesBufferOffset;

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

void VulkanMeshManager::DeleteMesh(MeshHandle)
{
	// todo: implement
}

/* Deletes staging buffer memory no longer in use.  Requires queue synchronization 
 * with the command buffer submitted via FlushLoadQueue() */
void VulkanMeshManager::Clean()
{
	while(!mStagingBuffers.empty())
	{
		StagingBuffer stagingBuffer = mStagingBuffers.top();
		vmaDestroyBuffer(mApplication->allocator, stagingBuffer.buffer, stagingBuffer.allocation);
		mStagingBuffers.pop();
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

VkDeviceSize SubmeshAllocation::GetVerticesOffset() const
{
	return(verticesOffset);
}

VkDeviceSize SubmeshAllocation::GetIndicesOffset() const
{
	return(indicesOffset);
}

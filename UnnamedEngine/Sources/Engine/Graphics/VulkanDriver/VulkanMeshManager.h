#pragma once
#include "Engine\Base\Common\Common.h"

#include <vulkan/vulkan.h>

#include <unordered_map>
#include <stack>

#include "Engine/Base/Resource/URI.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/ModelResource.h"

#include "vk_mem_alloc.h"

struct VulkanApplication;

#define NULL_MESH_HANDLE 0;

// todo: implement generational ids
typedef unsigned int MeshHandle;

struct MeshLayout
{
	uint64_t properties;
	uint32_t numUVs;
};

static MeshLayout UE_MESHLAYOUT_NORMALS_2UV = {
	HAS_NORMALS_BYTE_OFFSET,
	2
};

struct QueuedMeshLoad
{
	MeshHandle handle;
	std::shared_ptr<ModelResource> model;
};

struct SubmeshAllocation
{
	VkDeviceSize GetVerticesOffset() const;
	VkDeviceSize GetIndicesOffset() const;

	VmaAllocation allocation;
	VkBuffer buffer;

	VkDeviceSize verticesOffset;
	VkDeviceSize indicesOffset;
	uint32_t numIndices;

	// Mesh properties
	uint64_t meshFlags;
	uint32_t numUVs;
};

struct MeshInfo
{
	/* 
	 * MeshHandle can be NULL_MESH_HANDLE if the mesh is still in the load queue.
	 * Otherwise it equal to the index concatenated with the generational component 
	 * in the top 32 bits.
	 */
	MeshHandle handle;
	std::vector<SubmeshAllocation> submeshAllocations;
};

struct StagingBuffer
{
	VkBuffer buffer;
	VmaAllocation allocation;
};

// Note:  
class VulkanMeshManager
{
public:
	VulkanMeshManager(Ptr<ResourceManager> resManager, VulkanApplication* application);
	MeshHandle CreateMesh(URI resourceLocation);
	MeshHandle CreateMesh(ResourceType<ModelResource> res);
	MeshInfo GetMeshInfo(MeshHandle h);
	std::vector<SubmeshAllocation> GetMeshesWithLayout(const MeshLayout& meshLayout);

	void FlushLoadQueue(VkCommandBuffer buffer);
	void DeleteMesh(MeshHandle);
	void Clean();

private:

	MeshHandle GetFreeHandle();
	VulkanApplication* mApplication;

	std::unordered_map<URIHashType, MeshHandle> mHandleMap;
	std::vector<MeshInfo> mModels;

	std::stack<StagingBuffer> mStagingBuffers;

	// todo:  do we even need handles?
	std::stack<MeshHandle> mFreedHandles;
	
	// QueuedMeshLoad
	std::stack<QueuedMeshLoad> mLoadQueue;

	Ptr<ResourceManager> mResourceManager;
};
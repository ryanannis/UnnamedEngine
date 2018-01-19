#pragma once
#include "Engine\Base\Common\Common.h"

#include <vulkan/vulkan.h>

#include <unordered_map>
#include <stack>

#include "Engine/Base/Resource/URI.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/ModelResource.h"

#include "Engine/Graphics/VulkanDriver/VulkanApplication.h"

#define NULL_MESH_HANDLE 0;

// todo: implement generational ids
typedef uint64_t MeshHandle;

struct QueuedMeshLoad
{
	MeshHandle m;
	std::shared_ptr<ModelResource> model;
};

struct SubmeshAllocation
{
	VmaAllocation allocation;
	VkBuffer buffer;
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

// Note:  
class VulkanMeshManager
{
public:
	VulkanMeshManager(Ptr<ResourceManager> resManager, VulkanApplication* application);
	MeshHandle CreateMesh(URI resourceLocation);
	MeshHandle CreateMesh(ResourceType<ModelResource> res);
	MeshInfo GetMeshInfo(MeshHandle h);

	void FlushLoadQueue(VkCommandBuffer buffer);
	//void DeleteMesh(MeshHandle h);

private:

	MeshHandle GetFreeHandle();
	VulkanApplication* mApplication;

	std::unordered_map<URIHashType, MeshHandle> mHandleMap;
	std::vector<MeshInfo> mModels;

	// todo:  do we even need handles?
	std::stack<MeshHandle> mFreedHandles;
	
	// QueuedMeshLoad
	std::stack<QueuedMeshLoad> mLoadQueue;

	Ptr<ResourceManager> mResourceManager;
};
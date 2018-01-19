#pragma once
#include "Engine\Base\Common\Common.h"

#include <vulkan/vulkan.h>

#include <unordered_map>
#include <stack>

#include "Engine/Base/Resource/URI.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/ModelResource.h"

#include "Engine/Graphics/VulkanDriver/VulkanApplication.h"

// todo: implement generational ids here
typedef uint64_t MeshHandle;

struct SubmeshAllocation
{
	VmaAllocation allocation;
	VkBuffer buffer;
};

struct MeshInfo
{
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
	//void DeleteMesh(MeshHandle h);

private:

	MeshHandle GetFreeHandle();
	VulkanApplication* mApplication;

	std::unordered_map<URIHashType, MeshHandle> mHandleMap;
	std::vector<MeshInfo> mModels;

	// todo:  do we even need handles?
	std::stack<MeshHandle> mFreedHandles;

	Ptr<ResourceManager> mResourceManager;
};
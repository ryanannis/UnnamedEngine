#include "VulkanMeshManager.h"

#include "Engine/Base/Resource/ResourceManager.h"

VulkanMeshManager::VulkanMeshManager(Ptr<ResourceManager> resManager) :
	mResourceManager(resManager)
{}

MeshHandle VulkanMeshManager::CreateMesh(URI resourceLocation)
{
	return(CreateMesh(resourceLocation));
}

MeshHandle VulkanMeshManager::CreateMesh(ResourceType<ModelResource> res)
{
	auto existingResource = mHandleMap.find(res.GetURI().GetHash());
	if(existingResource != mHandleMap.end())
	{
		return(existingResource->second);
	}

	auto meshResource = mResourceManager->LoadResource(res);
	if(!meshResource)
	{
		// Failed to load shader!
		assert(false);
	}
	
	auto modelData = meshResource->GetMeshes();

	auto nextHandle = GetFreeHandle();
	mHandleMap.insert(std::make_pair(res.GetURI().GetHash(), nextHandle));

	// Actually create the shader

	return(nextHandle);

	// Let the mesh fall out of scope from the regular resource manager.
	// todo:  This is technically a waste of a smart ptr copy and alloc
}

MeshInfo VulkanMeshManager::GetMeshInfo(MeshHandle h)
{
	assert(h < mShaders.size());
	return(mShaders[h]);
}

MeshHandle VulkanMeshManager::GetFreeHandle()
{
	if(mFreedHandles.size() != 0)
	{
		auto nextHandle = mFreedHandles.top();
		mFreedHandles.pop();
		return(nextHandle);
	}

	MeshHandle nextNewHandle = mShaders.size();
	mShaders.resize(nextNewHandle + 1);

	return(nextNewHandle);
}

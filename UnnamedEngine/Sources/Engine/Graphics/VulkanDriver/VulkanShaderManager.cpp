#include "VulkanShaderManager.h"

#include "Engine/Base/Resource/ResourceManager.h"

VulkanShaderManager::VulkanShaderManager(Ptr<ResourceManager> resManager) :
	mResourceManager(resManager)
{}

ShaderHandle VulkanShaderManager::CreateShaderModule(URI resourceLocation)
{
	return(CreateShaderModule(resourceLocation));
}

ShaderHandle VulkanShaderManager::CreateShaderModule(ResourceType<ShaderResource> res)
{
	auto existingResource = mHandleMap.find(res.GetURI().GetHash());
	if(existingResource != mHandleMap.end())
	{
		return(existingResource->second);
	}

	auto shaderResource = mResourceManager->LoadResource(res);
	if(!shaderResource)
	{
		// Failed to load shader!
		assert(false);
	}

	auto shaderBytecode = shaderResource->GetShaderBinary();

	auto nextHandle = GetFreeHandle();
	mHandleMap.insert(std::make_pair(res.GetURI().GetHash(), nextHandle));

	// Actually create the shader
	VkShaderModule module;
	VkShaderModuleCreateInfo shaderModule;
	shaderModule.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModule.codeSize = shaderBytecode.size();
	shaderModule.pCode = reinterpret_cast<const uint32_t*>(shaderBytecode.data());

	if(vkCreateShaderModule(
		mApplication->logicalDevice,
		&shaderModule,
		nullptr,
		&module
	) != VK_SUCCESS)
	{
		assert(false);
	}

	auto& shaderInfo = mShaders[nextHandle];
	shaderInfo.module = module;
	shaderInfo.type = shaderResource->GetShaderType();

	return(nextHandle);
	// Let the shader fall out of scope from the regular resource manager.
	// todo:  This is technically a waste of a smart ptr copy and alloc
}

ShaderHandle VulkanShaderManager::GetShaderModule(ShaderHandle h)
{
	assert(h < mShaders.size());
	return(mShaders[h].module);
}

/*
void VulkanShaderManager::DeleteShaderModule(ShaderHandle h)
{
}
*/

VkPipelineShaderStageCreateInfo VulkanShaderManager::GetShaderPipelineInfo(ShaderHandle h)
{
	VkPipelineShaderStageCreateInfo shaderStage;
	shaderStage.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
	shaderStage.module = mShaders[h].module;
	shaderStage.pName = "main";
	shaderStage.pSpecializationInfo = nullptr;
	
	return(shaderStage);
}

ShaderHandle VulkanShaderManager::GetFreeHandle()
{
	if(mFreedHandles.size() != 0)
	{
		auto nextHandle = mFreedHandles.top();
		mFreedHandles.pop();
		return(nextHandle);
	}

	ShaderHandle nextNewHandle = mShaders.size();
	mShaders.resize(nextNewHandle + 1);

	return(nextNewHandle);
}

#include "VulkanShaderManager.h"

#include "Engine/Base/Resource/ResourceManager.h"
#include "Engine/Graphics/VulkanDriver/VulkanApplication.h"

VulkanShaderManager::VulkanShaderManager(Ptr<ResourceManager> resManager, VulkanApplication* application) :
	mResourceManager(resManager),
	mApplication(application)
{}

ShaderHandle VulkanShaderManager::CreateShaderModule(URI resourceLocation)
{
	return(CreateShaderModule(ResourceType<ShaderResource>(resourceLocation)));
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
	VkShaderModule module;;
	VkShaderModuleCreateInfo shaderModule = {};
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

VkShaderModule VulkanShaderManager::GetShaderModule(ShaderHandle h)
{
	assert(h < mShaders.size());
	return(mShaders[h].module);
}

VkPipelineShaderStageCreateInfo VulkanShaderManager::GetShaderPipelineInfo(ShaderHandle h)
{
	ShaderInfo shaderInfo = mShaders[h];
	
	VkShaderStageFlagBits stage;
	if(shaderInfo.type == ShaderType::VERTEX)
	{
		stage = VK_SHADER_STAGE_VERTEX_BIT;
	}
	else if(shaderInfo.type == ShaderType::FRAGMENT)
	{
		stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	}
	else if(shaderInfo.type == ShaderType::GEOMETRY)
	{
		stage = VK_SHADER_STAGE_GEOMETRY_BIT;
	}
	else if(shaderInfo.type == ShaderType::COMPUTE)
	{
		stage = VK_SHADER_STAGE_COMPUTE_BIT;
	}

	VkPipelineShaderStageCreateInfo shaderStage = {};
	shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStage.stage = stage;
	shaderStage.module = mShaders[h].module;
	shaderStage.pName = "main";
	
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

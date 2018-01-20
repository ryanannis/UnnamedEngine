#pragma once
#include "Engine\Base\Common\Common.h"

#include <vulkan/vulkan.h>

#include <unordered_map>
#include <stack>

#include "Engine/Base/Resource/URI.h"
#include "Engine/Base/Resource/ResourceType.h"
#include "Engine/Base/Resource/ShaderResource.h"

struct VulkanApplication;

// todo: implement generational ids here
typedef unsigned int ShaderHandle;

struct ShaderInfo
{
	VkShaderModule module;
	ShaderType type;
};

// Note:  
class VulkanShaderManager
{
public:
	VulkanShaderManager(Ptr<ResourceManager> resManager, VulkanApplication* application);
	ShaderHandle CreateShaderModule(URI resourceLocation);
	ShaderHandle CreateShaderModule(ResourceType<ShaderResource> res);
	VkShaderModule GetShaderModule(ShaderHandle h);
	//void DeleteShaderModule(ShaderHandle h);
	VkPipelineShaderStageCreateInfo GetShaderPipelineInfo(ShaderHandle h);

private:
	ShaderHandle GetFreeHandle();

	VulkanApplication* mApplication;

	std::unordered_map<URIHashType, ShaderHandle> mHandleMap;
	std::vector<ShaderInfo> mShaders;

	// todo:  do we even need handles?
	std::stack<ShaderHandle> mFreedHandles;

	Ptr<ResourceManager> mResourceManager;
};
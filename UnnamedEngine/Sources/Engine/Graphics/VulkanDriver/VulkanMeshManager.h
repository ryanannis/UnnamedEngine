#pragma once
#include "Engine/Base/Common/Common.h"

#include <vulkan\vulkan.h>

#include "Formats/MeshFormat.h"

class VulkanMeshManager
{
public:
	VkVertexInputBindingDescription GetBindingDescriptionForMesh(SubmeshData* data);
	std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();

	uint32_t GetVertexBuffer(uint32_t numBytes);

private:
	void AllocateMemory();
	std::vector<VkBuffer> mFreeVertexBuffers;
};
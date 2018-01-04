#include  "VulkanMeshManager.h"

VkVertexInputBindingDescription VulkanMeshManager::GetBindingDescriptionForMesh(SubmeshData* data)
{
	VkVertexInputBindingDescription bindingDescription = {};
	bindingDescription.binding = 0;
	bindingDescription.stride = data->GetInterleavedSize();
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	return(bindingDescription);
}

std::array<VkVertexInputAttributeDescription, 2> VulkanMeshManager::GetAttributeDescriptions()
{
	//todo: find good way enumerate different formats at runtime without mallocing every time
	std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

	// Position
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = 0;

	// Normals
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = sizeof(Vector3f);

	return(attributeDescriptions);
}

uint32_t VulkanMeshManager::GetVertexBuffer(uint32_t numBytes)
{
	return uint32_t();
}

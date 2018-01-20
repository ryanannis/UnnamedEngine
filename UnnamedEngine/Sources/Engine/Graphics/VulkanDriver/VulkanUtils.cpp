#include "VulkanUtils.h"

VulkanUtils::Mesh::SubmeshBindingDescription VulkanUtils::Mesh::ComputeSubmeshBindingDescription(SubmeshData* submesh)
{
	VkVertexInputBindingDescription vertexBindingDescription;
	vertexBindingDescription.binding = VERTEX_BINDING_OFFSET;
	vertexBindingDescription.stride = submesh->GetInterleavedSize();
	vertexBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	std::vector<VkVertexInputAttributeDescription> vertexAttributes(submesh->GetNumberLocations());

	/* Todo:  Make this more safe? */
	uint32_t currentOffset = 0;
	uint32_t currentLocation = 0;

	// Mesh Vertices
	VkVertexInputAttributeDescription verticeVertexAttributeDescriptions = {};
	verticeVertexAttributeDescriptions.location = currentLocation;
	verticeVertexAttributeDescriptions.binding = vertexBindingDescription.binding;
	verticeVertexAttributeDescriptions.format = VK_FORMAT_R32G32B32_SFLOAT;
	verticeVertexAttributeDescriptions.offset = currentOffset;

	currentLocation += 1;
	currentOffset += 3 * sizeof(float);
	
	// Normals
	if(submesh->properties & HAS_NORMALS_BYTE_OFFSET)
	{
		VkVertexInputAttributeDescription verticeNormalAttributeDescriptions = {};
		verticeNormalAttributeDescriptions.location = currentLocation;
		verticeNormalAttributeDescriptions.binding = vertexBindingDescription.binding;
		verticeNormalAttributeDescriptions.format = VK_FORMAT_R32G32B32_SFLOAT;
		verticeNormalAttributeDescriptions.offset = currentOffset;

		currentLocation += 1;
		currentOffset += 3 * sizeof(float);
	}	

	// UVs
	for(uint32_t i = 0; i < submesh->numUVs; i++)
	{
		VkVertexInputAttributeDescription verticeUVAttributeDescriptions = {};
		verticeUVAttributeDescriptions.location = currentLocation;
		verticeUVAttributeDescriptions.binding = vertexBindingDescription.binding;
		verticeUVAttributeDescriptions.format = VK_FORMAT_R32G32_SFLOAT;
		verticeUVAttributeDescriptions.offset = currentOffset;

		currentLocation += 1;
		currentOffset += 3 * sizeof(float);
	}

	VulkanUtils::Mesh::SubmeshBindingDescription submeshBinding;
	submeshBinding.vertexBinding = vertexBindingDescription;
	submeshBinding.attributeBindings = vertexAttributes;

	return(submeshBinding);
}
#pragma once
#include "Engine/Base/Common/Common.h"
#include <vulkan\vulkan.h>
#include "Formats/MeshFormat.h"

#define NO_CUSTOM_ALLOCATOR nullptr

//todo:  remove this when we create a pipeline manager and make big buffers
#define VERTEX_BINDING_OFFSET 0

namespace VulkanUtils
{
	namespace Mesh
	{
		struct SubmeshBindingDescription
		{
			VkVertexInputBindingDescription vertexBinding;
			std::vector<VkVertexInputAttributeDescription> attributeBindings;
		};

		SubmeshBindingDescription ComputeSubmeshBindingDescription(SubmeshData* submesh);
	}
}
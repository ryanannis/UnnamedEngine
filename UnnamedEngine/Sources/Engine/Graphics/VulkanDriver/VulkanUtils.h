#pragma once
#include "Engine/Base/Common/Common.h"
#include <vulkan\vulkan.h>
#include "Formats/MeshFormat.h"

#define NO_CUSTOM_ALLOCATOR nullptr

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
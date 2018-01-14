#pragma once
#include "Engine/Base/Common/Common.h"
#include <vulkan\vulkan.h>
#include "Formats/MeshFormat.h"

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
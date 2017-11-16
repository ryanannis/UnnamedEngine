#include "RenderComponent.h"
#include "Engine/Base/Resource/Serializer.h"
#include "Engine/Base/Resource/PropTree.h"
#include "Engine/Base/Resource/ResourceManager.h"

STATICDEF(RenderComponent, "RenderComponent");

RenderComponent::RenderComponent() :
	mesh{},
	handle{GraphicsHandle{0}}
{}

Serializer& RenderComponent::Serialize(Serializer& s)
{
	s.Serialize("Name", mesh);
	return(s);
}

void RenderComponent::Deserialize(const DeserializationData& s)
{
	assert(s.GetResourceManager());

	const PropTree& t = s.GetProps();	
	auto nameIt = t.leaves.find("Mesh");

	if(nameIt != t.leaves.end())
	{
		auto resName = nameIt->second.GetAsString();
		mesh = ResourceType<MeshResource>(resName);
		s.GetResourceManager()->LoadResource(mesh);
	}
}


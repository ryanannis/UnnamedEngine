#include "RenderComponent.h"
#include "Engine/Base/Resource/Serializer.h"
#include "Engine/Base/Resource/PropTree.h"

STATICREGISTER(RenderComponent, "RenderComponent");

RenderComponent::RenderComponent() :
	mesh{}
{}

Serializer& RenderComponent::Serialize(Serializer& s)
{
	s.Serialize("Name", mesh);
	return(s);
}

void RenderComponent::Deserialize(PropTree& t)
{
	auto nameIt = t.leaves.find("Name");

	if(nameIt != t.leaves.end())
	{
		 mesh = ResourceType<MeshResource>(nameIt->second.GetAsString());
	}
}
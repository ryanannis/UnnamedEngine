#include "RenderComponent.h"
#include "Engine/Base/Resource/Serializer.h"
#include "Engine/Base/Resource/PropTree.h"
#include "Engine/Base/Resource/ResourceManager.h"

STATICREGISTER(RenderComponent, "RenderComponent");

RenderComponent::RenderComponent() :
	mesh{},
	handle{GraphicsHandle{0}}
{}

Serializer& RenderComponent::Serialize(Serializer& s)
{
	SERIALIZE(s, "Mesh", mesh);
	return(s);
}

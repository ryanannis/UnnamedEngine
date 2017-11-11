#include "StaticComponent.h"
#include "Engine/Base/Types/Component.h"
#include "Engine/Base/Managers/EntityAdmin.h"

BaseComponent* CreateComponent(const std::string& name, Entity& entity, EntityAdmin& admin)
{
	const auto& registry = GetStaticRegistry();
	const auto& componentIt = registry.find(name);

	assert(componentIt != registry.end()); // Tried to create invalid component;
	
	auto createFunc = componentIt->second.createFunction;
	return(createFunc(entity, admin));
}

#include "StaticComponent.h"
#include "Engine/Base/Types/Component.h"

std::shared_ptr<ComponentBase> StaticReg::StaticCreateComponent(const std::string& name)
{
	const auto& registry = GetStaticRegistry();
	const auto& componentIt = registry.find(name);

	assert(componentIt != registry.end()); // Tried to create invalid component;

	auto createFunc = componentIt->second.createFunction;
	return(createFunc());
}

Ptr<ComponentBase> StaticReg::StaticCreateRegisterComponent(const std::string& name, Entity& entity, EntityAdmin& admin)
{
	const auto& registry = GetStaticRegistry();
	const auto& componentIt = registry.find(name);

	assert(componentIt != registry.end()); // Tried to create invalid component;
	
	auto createFunc = componentIt->second.createAndRegisterFunction;

	return(createFunc(entity, admin));
}

// Static Registry Singleton
std::unordered_map<std::string, StaticReg::StaticRegistryEntry>& StaticReg::GetStaticRegistry()
{
	static std::unordered_map<std::string, StaticRegistryEntry> staticRegistry;
	return(staticRegistry);
}
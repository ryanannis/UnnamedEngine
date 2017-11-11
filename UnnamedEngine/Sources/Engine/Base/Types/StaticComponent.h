#pragma once
#include <unordered_map>
#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Common/Ptr.h"

class BaseComponent;
class EntityAdmin;

// Include this header in every component that needs to be statically registerered
// NOT Component or BaseComponent as that will cause circular dependencies

//////////////////////////////////////////////////////////////////////
/////////////////////STATIC REGISTRATION//////////////////////////////
//////////////////////////////////////////////////////////////////////

BaseComponent* CreateComponent(const std::string& name, Entity& entity, EntityAdmin& admin);

// Contains all the information necessary to 
struct StaticRegistryEntry
{
public:
	// Contains type information to forward to EntityAdmin
	Ptr<BaseComponent> (*createFunction)(Entity, EntityAdmin&);
	// We create data-defined types from the copy constructor of a singleton-loaded component
	// to prevent deserializing multiple times
	void (*copyConstructor)(void*, void*);
};

// Static Registry Singleton
std::unordered_map<std::string, StaticRegistryEntry>& GetStaticRegistry()
{
	static std::unordered_map<std::string, StaticRegistryEntry> staticRegistry;
	return(staticRegistry);
}

// Generic class which creates a component and adds it to the given
template <class T>
Ptr<ComponentBase> CreateAndRegisterComponent(Entity entity, EntityAdmin& admin)
{
	return(admin.AddComponent(entity));
}

// A wrapper so we can statically refer to the copy operator of each
// derived component
// todo: give option for EntityAdmin to be lazy in initialization the
// component and use the copy constructor instead
template <class T>
void CopyComponent(void* copyTo, const T* copyFrom)
{
	T& to = *(static_cast<T*>(copyTo));
	T& from = *(static<T*>(copyFrom));
	copyTo = copyFrom;
}

template<class T>
struct RegistryEntry
{
public:
	// The entry for a given component type is a singleton
	static RegistryEntry<T>& Instance(const std::string& name)
	{
		static RegistryEntry<T> inst(name);
		return(inst);
	}

private:
	RegistryEntry(const std::string& name)
	{
		const auto& staticRegistry = GetStaticRegistry();
		StaticRegistryEntry entry;
		entry.CreateFunction = &CreateAndRegisterComponent<T>;
		entry.copyConstructor = &C

		auto registerEntry = reg.insert(std:pair<std::string, StaticRegistryEntry>(name, func));

		// tried to register two components with same name
		assert(!regEntry.second);
	}
};

#define STATICREGISTER(TYPE, NAME)                                    \
template<class T>                                                     \
class StaticRegistration;                                             \
                                                                      \
template<>                                                            \
class StaticRegistration<TYPE>                                        \
{                                                                     \
    static const RegistryEntry<TYPE>& reg;                            \
};                                                                    \
                                                                      \
const RegistryEntry<TYPE>& StaticRegistration<TYPE>::reg =            \
	RegistryEntry<TYPE>::Instance(NAME);                              \
		  
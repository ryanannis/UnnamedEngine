#pragma once

#include <unordered_map>
#include <utility>

#include "Engine/Base/Types/Entity.h"
#include "Engine/Base/Common/Ptr.h"
#include "Engine/Base/Managers/EntityAdmin.h"

class ComponentBase;

// Include this header in every component that needs to be statically registerered
// DO NOT include Component or ComponentBase as that will cause circular dependencies

//////////////////////////////////////////////////////////////////////
/////////////////////STATIC COMPONENT REGISTRATION////////////////////
//////////////////////////////////////////////////////////////////////

namespace StaticReg
{
	struct StaticRegistryEntry
	{
	public:
		// Contains type information to forward to EntityAdmin
		Ptr<ComponentBase>(*createAndRegisterFunction)(Entity, EntityAdmin&);
		std::unique_ptr<ComponentBase>(*createFunction)();
		
		// We create data-defined types from the copy constructor of a singleton-loaded component
		// to prevent deserializing multiple times
		void (*copyConstructor)(void*, void*);

		// Not all places where this is needed necessarily has type information
		ComponentFlag flag;
		size_t memSize;
		StorageStrategy storageStrategy;
	};

	// Static Registry Singleton
	std::unordered_map<std::string, StaticRegistryEntry>& GetStaticRegistry();

	// Generic function which creates a component and adds it to the given admin
	template <class T>
	Ptr<ComponentBase> CreateAndRegisterComponent(Entity entity, EntityAdmin& admin)
	{
		return(admin.AddComponent<T>(entity));
	}

	// Generic function which creates a component on the heap
	template <class T>
	std::unique_ptr<ComponentBase> CreateComponent()
	{
		return(std::unique_ptr<ComponentBase>(new T));
	}

	// A wrapper so we can statically refer to the copy operator of each
	// derived component
	// todo: give option for EntityAdmin to be lazy in initialization the
	// component and use the copy constructor instead
	template <class T>
	void CopyComponent(void* copyTo, void* copyFrom)
	{
		T& to = *(static_cast<T*>(copyTo));
		T& from = *(static_cast<T*>(copyFrom));
		to = from;
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
			auto& staticRegistry = GetStaticRegistry();
			StaticRegistryEntry entry;
			entry.createAndRegisterFunction = &CreateAndRegisterComponent<T>;
			entry.copyConstructor = &CopyComponent<T>;
			entry.createFunction = &CreateComponent<T>;
			T::RegisterGroup();
			entry.flag = ComponentGroup<T>();
			entry.memSize = sizeof(T);
			// waste of (a trivial amount) of bytes
			entry.storageStrategy = T::GetStorageStrategy();

			auto ret = staticRegistry.try_emplace(name, std::move(entry));
			assert(ret.second);
		}
	};

	// Save this to do fast initialization without a map lookup
	Ptr<const StaticRegistryEntry> GetComponentRegistryInformation(const std::string& name);

	// Creates a component on the heap
	std::unique_ptr<ComponentBase> StaticCreateComponent(const std::string& name);

	// Create a component and register it with entity on admin
	Ptr<ComponentBase> StaticCreateRegisterComponent(const std::string& name, Entity& entity, EntityAdmin& admin);

}


#define STATICREGISTER(TYPE, NAME)                                    \
namespace StaticReg													  \
{																	  \
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
}																	  \

